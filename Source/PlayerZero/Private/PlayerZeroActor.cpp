// Copyright Ready Player Me

#include "PlayerZeroActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "glTFRuntimeSkeletalMeshComponent.h"
#include "PlayerZero.h"
#include "PlayerZeroCharacterTypes.h"
#include "Api/Assets/AssetApi.h"

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
	LoadAsset(FRpmAsset(), GltfAsset);
}

<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
void APlayerZeroActor::LoadAsset(const FAsset& Asset, UglTFRuntimeAsset* GltfAsset)
=======
void ARpmActor::LoadAsset(const FRpmAsset& Asset, UglTFRuntimeAsset* GltfAsset)
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
{
	if (!GltfAsset)
	{
		UE_LOG(LogGLTFRuntime, Warning, TEXT("No asset to setup"));
		return;
	}
	if(Asset.Type == FAssetApi::CharacterStyleAssetType)
	{
		CharacterData.CharacterStyleId = Asset.Id;
		if(AnimationConfigsByCharacterStyleId.Contains(CharacterData.CharacterStyleId))
		{
			AnimationConfig = AnimationConfigsByCharacterStyleId[CharacterData.CharacterStyleId];
			SkeletalMeshConfig.Skeleton =  AnimationConfig.Skeleton;
			SkeletalMeshConfig.SkeletonConfig.CopyRotationsFrom =  AnimationConfig.Skeleton;
		}
	}
	RemoveMeshComponentsOfType(Asset.Type);
	double LoadingStartTime = FPlatformTime::Seconds();

	const TArray<USceneComponent*> NewMeshComponents = LoadMeshComponents(GltfAsset, Asset.Type);
	if (NewMeshComponents.Num() > 0)
	{
		LoadedMeshComponentsByAssetType.Add(Asset.Type, NewMeshComponents);
		if(AnimationConfigsByCharacterStyleId.Contains(CharacterData.CharacterStyleId))
		{
			if (MasterPoseComponent == nullptr)
			{
<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
				UE_LOG(LogPlayerZero, Error, TEXT("MasterPoseComponent is null for base model %s"), *CharacterData.BaseModelId);
=======
				UE_LOG(LogReadyPlayerMe, Error, TEXT("MasterPoseComponent is null for base model %s"), *CharacterData.CharacterStyleId);
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
				return;
			}

			if (!AnimationConfig.AnimationBlueprint)
			{
<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
				UE_LOG(LogPlayerZero, Error, TEXT("AnimationBlueprint is null for base model %s"), *CharacterData.BaseModelId);
=======
				UE_LOG(LogReadyPlayerMe, Error, TEXT("AnimationBlueprint is null for base model %s"), *CharacterData.CharacterStyleId);
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
				return;
			}
			
			MasterPoseComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			MasterPoseComponent->SetAnimClass(AnimationConfig.AnimationBlueprint);
<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
			UE_LOG(LogPlayerZero, Log, TEXT("Set Animation Blueprint for %s"), *CharacterData.BaseModelId);
=======
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
		}

		UE_LOG(LogPlayerZero, Log, TEXT("Asset loaded in %f seconds"), FPlatformTime::Seconds() - LoadingStartTime);
		return;
	}
	
	UE_LOG(LogPlayerZero, Error, TEXT("Failed to load mesh components"));
}

<<<<<<< HEAD:Source/PlayerZero/Private/PlayerZeroActor.cpp
void APlayerZeroActor::LoadGltfAssetWithSkeleton(UglTFRuntimeAsset* GltfAsset, const FAsset& Asset, const FPlayerZeroAnimationConfig& InAnimationCharacter)
=======
void ARpmActor::LoadGltfAssetWithSkeleton(UglTFRuntimeAsset* GltfAsset, const FRpmAsset& Asset, const FRpmAnimationConfig& InAnimationCharacter)
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmActor.cpp
{
	AnimationConfig = InAnimationCharacter;
	SkeletalMeshConfig.Skeleton =  AnimationConfig.Skeleton;
	SkeletalMeshConfig.SkeletonConfig.CopyRotationsFrom =  AnimationConfig.Skeleton;
	LoadAsset(Asset, GltfAsset);
}

void APlayerZeroActor::RemoveMeshComponentsOfType(const FString& AssetType)
{
	if (LoadedMeshComponentsByAssetType.IsEmpty())
	{
		UE_LOG( LogPlayerZero, Log, TEXT("No mesh components to remove"));
		return;
	}

	// Remove components by type, or remove all if AssetType is empty or it's a new base model
	if (AssetType.IsEmpty() || AssetType == FAssetApi::CharacterStyleAssetType)
	{
		RemoveAllMeshes();
	}
	else if (LoadedMeshComponentsByAssetType.Contains(AssetType))
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
	bool bIsMasterPoseUpdateRequired = AssetType == FAssetApi::CharacterStyleAssetType || AssetType.IsEmpty();
	
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
