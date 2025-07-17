// Copyright Ready Player Me

#include "PlayerZeroActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "glTFRuntimeSkeletalMeshComponent.h"
#include "RpmNextGen.h"
#include "RpmCharacterTypes.h"

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
	if(AnimationConfigsByCharacterStyleId.Contains(CharacterData.CharacterStyleId))
	{
		AnimationConfig = AnimationConfigsByCharacterStyleId[CharacterData.CharacterStyleId];
		SkeletalMeshConfig.Skeleton =  AnimationConfig.Skeleton;
		SkeletalMeshConfig.SkeletonConfig.CopyRotationsFrom =  AnimationConfig.Skeleton;
	}
	// TODO cleanup
	//LoadAsset(FRpmAsset(), GltfAsset);
}

void ARpmActor::LoadGltfAssetWithSkeleton(UglTFRuntimeAsset* GltfAsset, const FRpmAsset& Asset, const FRpmAnimationConfig& InAnimationCharacter)
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
{
	AnimationConfig = InAnimationCharacter;
	SkeletalMeshConfig.Skeleton =  AnimationConfig.Skeleton;
	SkeletalMeshConfig.SkeletonConfig.CopyRotationsFrom =  AnimationConfig.Skeleton;
	// TODO cleanup
	//LoadAsset(Asset, GltfAsset);
}

void APlayerZeroActor::RemoveMeshComponentsOfType(const FString& AssetType)
{
	if (LoadedMeshComponentsByAssetType.IsEmpty())
	{
		UE_LOG( LogPlayerZero, Log, TEXT("No mesh components to remove"));
		return;
	}
	// TODO cleanup
	// // Remove components by type, or remove all if AssetType is empty or it's a new base model
	// if (AssetType.IsEmpty() || AssetType == FAssetApi::CharacterStyleAssetType)
	// {
	// 	RemoveAllMeshes();
	// }
	// else if (LoadedMeshComponentsByAssetType.Contains(AssetType))
	// {
	// 	TArray<USceneComponent*>& ComponentsToRemove = LoadedMeshComponentsByAssetType[AssetType];
	// 	for (USceneComponent* ComponentToRemove : ComponentsToRemove)
	// 	{
	// 		if (ComponentToRemove)
	// 		{
	// 			ComponentToRemove->DestroyComponent();
	// 			ComponentToRemove = nullptr;
	// 		}
	// 	}
	// 	LoadedMeshComponentsByAssetType.Remove(AssetType);
	// }
}

void ARpmActor::RemoveAssetOfType(const FRpmAsset& Asset)
{
	CharacterData.Assets.Remove(Asset.Type);
	RemoveMeshComponentsOfType(Asset.Type);
}

void APlayerZeroActor::RemoveAllMeshes()
{
	for (const auto& Pair : LoadedMeshComponentsByAssetType)
	{
		RemoveMeshComponents(Pair.Value);
	}
	LoadedMeshComponentsByAssetType.Empty();
}

<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
TArray<USceneComponent*> APlayerZeroActor::LoadMeshComponents(UglTFRuntimeAsset* GltfAsset, const FString& AssetType)
=======
void ARpmActor::RemoveMeshComponents(const TArray<USceneComponent*>& Components)
{
	for (USceneComponent* Component : Components)
	{
		if (Component)
		{
			Component->DestroyComponent();
			Component = nullptr;
		}
	}
}

TArray<USceneComponent*> ARpmActor::LoadMeshComponents(UglTFRuntimeAsset* GltfAsset, const FString& AssetType)
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
{
	TArray<FglTFRuntimeNode> AllNodes = GltfAsset->GetNodes();
	TArray<USceneComponent*> NewMeshComponents;
	// TODO cleanup
	// bool bIsMasterPoseUpdateRequired = AssetType == FAssetApi::CharacterStyleAssetType || AssetType.IsEmpty();
	bool bIsMasterPoseUpdateRequired = false;
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
<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
				UE_LOG( LogPlayerZero, Log, TEXT("Setting master pose component"));
=======
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
				MasterPoseComponent = SkeletalMeshComponent;
				NewMeshComponents.Add(SkeletalMeshComponent);
				bIsMasterPoseUpdateRequired = false;
				continue;
			}
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
			SkeletalMeshComponent->SetLeaderPoseComponent(MasterPoseComponent.Get());
#else
			SkeletalMeshComponent->SetMasterPoseComponent(MasterPoseComponent.Get());
#endif
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
	if (!SkeletalMesh)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load skeletal mesh for node %s"), *Node.Name);
		return nullptr;
	}
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
<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp

void APlayerZeroActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
=======
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
