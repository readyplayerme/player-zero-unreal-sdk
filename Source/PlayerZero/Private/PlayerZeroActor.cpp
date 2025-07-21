// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "glTFRuntimeSkeletalMeshComponent.h"
#include "PlayerZero.h"
#include "PlayerZeroCharacterTypes.h"

APlayerZeroActor::APlayerZeroActor()
{
	PrimaryActorTick.bCanEverTick = false;
	AssetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AssetRoot"));
	RootComponent = AssetRoot;
	MasterPoseComponent = nullptr;
}

void APlayerZeroActor::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerZeroActor::LoadCharacter(const FPlayerZeroCharacterData& InCharacterData, UglTFRuntimeAsset* GltfAsset)
{
	CharacterData = InCharacterData;
	if(AnimationConfigsByBaseModelId.Contains(CharacterData.BaseModelId))
	{
		AnimationConfig = AnimationConfigsByBaseModelId[CharacterData.BaseModelId];
		SkeletalMeshConfig.Skeleton =  AnimationConfig.Skeleton;
		SkeletalMeshConfig.SkeletonConfig.CopyRotationsFrom =  AnimationConfig.Skeleton;
	}
}

void APlayerZeroActor::RemoveMeshComponentsOfType(const FString& AssetType)
{
	if (LoadedMeshComponentsByAssetType.IsEmpty())
	{
		UE_LOG( LogPlayerZero, Log, TEXT("No mesh components to remove"));
		return;
	}
	if (LoadedMeshComponentsByAssetType.Contains(AssetType))
	{
		TArray<USceneComponent*>& ComponentsToRemove = LoadedMeshComponentsByAssetType[AssetType];
		for (USceneComponent* ComponentToRemove : ComponentsToRemove)
		{
			if (ComponentToRemove)
			{
				ComponentToRemove->DestroyComponent();
				ComponentToRemove = nullptr;
			}
		}
		LoadedMeshComponentsByAssetType.Remove(AssetType);
	}
}


void APlayerZeroActor::RemoveAllMeshes()
{
	for (const auto Pairs : LoadedMeshComponentsByAssetType){
		
		TArray<USceneComponent*> ComponentsToRemove = Pairs.Value;
		for (USceneComponent* ComponentToRemove : ComponentsToRemove)
		{
			if (ComponentToRemove)
			{
				ComponentToRemove->DestroyComponent();
				ComponentToRemove = nullptr;
			}
		}
	}
	LoadedMeshComponentsByAssetType.Empty();
}

TArray<USceneComponent*> APlayerZeroActor::LoadMeshComponents(UglTFRuntimeAsset* GltfAsset, const FString& AssetType)
{
	TArray<FglTFRuntimeNode> AllNodes = GltfAsset->GetNodes();
	TArray<USceneComponent*> NewMeshComponents;

	//TODO cleanup
	//if baseModel or full character asset changes we need to update master pose component
	//bool bIsMasterPoseUpdateRequired = AssetType == FAssetApi::BaseModelType || AssetType.IsEmpty();
	bool bIsMasterPoseUpdateRequired = true;
	// Loop through all nodes to create mesh components
	for (const FglTFRuntimeNode& Node : AllNodes)
	{
		// Skip bones and armature
		if(GltfAsset->NodeIsBone(Node.Index) || Node.Name.Contains("Armature"))
		{
			continue;
		}
		
		if (Node.SkinIndex >= 0)
		{
			USkeletalMeshComponent* SkeletalMeshComponent = CreateSkeletalMeshComponent(GltfAsset, Node);
			if(bIsMasterPoseUpdateRequired)
			{
				UE_LOG( LogPlayerZero, Log, TEXT("Setting master pose component"));
				MasterPoseComponent = SkeletalMeshComponent;
				NewMeshComponents.Add(SkeletalMeshComponent);
				bIsMasterPoseUpdateRequired = false;
				continue;
			}
			SkeletalMeshComponent->SetMasterPoseComponent(MasterPoseComponent.Get());
			NewMeshComponents.Add(SkeletalMeshComponent);
		}
		else
		{
			NewMeshComponents.Add(CreateStaticMeshComponent(GltfAsset, Node));

		}
	}
	return NewMeshComponents;
}

USkeletalMeshComponent* APlayerZeroActor::CreateSkeletalMeshComponent(UglTFRuntimeAsset* GltfAsset, const FglTFRuntimeNode& Node)
{
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	if (SkeletalMeshConfig.bPerPolyCollision)
	{
		SkeletalMeshComponent = NewObject<UglTFRuntimeSkeletalMeshComponent>(this, GetSafeNodeName<UglTFRuntimeSkeletalMeshComponent>(Node));
		SkeletalMeshComponent->bEnablePerPolyCollision = true;
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, GetSafeNodeName<USkeletalMeshComponent>(Node));
	}

	USkeletalMesh* SkeletalMesh = GltfAsset->LoadSkeletalMesh(Node.MeshIndex, Node.SkinIndex, SkeletalMeshConfig);
	SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
	SkeletalMeshComponent->SetupAttachment(AssetRoot);
	SkeletalMeshComponent->SetRelativeTransform(Node.Transform);
	SkeletalMeshComponent->RegisterComponent();
	AddInstanceComponent(SkeletalMeshComponent);
	
	return SkeletalMeshComponent;
}

UStaticMeshComponent* APlayerZeroActor::CreateStaticMeshComponent(UglTFRuntimeAsset* GltfAsset, const FglTFRuntimeNode& Node)
{
	UStaticMeshComponent* StaticMeshComponent = nullptr;
	TArray<FTransform> GPUInstancingTransforms;

	if (GltfAsset->GetNodeGPUInstancingTransforms(Node.Index, GPUInstancingTransforms))
	{
		UInstancedStaticMeshComponent* InstancedStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, GetSafeNodeName<UInstancedStaticMeshComponent>(Node));
		for (const FTransform& GPUInstanceTransform : GPUInstancingTransforms)
		{
			InstancedStaticMeshComponent->AddInstance(GPUInstanceTransform);
		}
		StaticMeshComponent = InstancedStaticMeshComponent;
	}
	else
	{
		StaticMeshComponent = NewObject<UStaticMeshComponent>(this, GetSafeNodeName<UStaticMeshComponent>(Node));
	}
	
	UStaticMesh* StaticMesh = GltfAsset->LoadStaticMeshLODs({Node.MeshIndex}, StaticMeshConfig);
	StaticMeshComponent->SetStaticMesh(StaticMesh);
	StaticMeshComponent->SetupAttachment(AssetRoot);
	StaticMeshComponent->SetRelativeTransform(Node.Transform);
	StaticMeshComponent->RegisterComponent();
	AddInstanceComponent(StaticMeshComponent);
	
	return StaticMeshComponent;
}

void APlayerZeroActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
