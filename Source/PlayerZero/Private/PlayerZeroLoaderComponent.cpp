#include "PlayerZeroLoaderComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "PlayerZeroSubsystem.h"

UPlayerZeroLoaderComponent::UPlayerZeroLoaderComponent()
{
	TargetSkeleton = nullptr;
	TargetMeshComponent = nullptr;
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerZeroLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!TargetMeshComponent)
	{
		TargetMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	}

	Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>();
	OnGltfAssetLoaded.BindUFunction(this, FName("ReplaceMeshWithGltfAsset"));
}

void UPlayerZeroLoaderComponent::LoadAvatar()
{
	if (AvatarId.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarLoaderComponent: No AvatarId set."));
		return;
	}

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarLoaderComponent: PlayerZeroSubsystem not found."));
		return;
	}
	
	Subsystem->LoadAvatarAsset(AvatarId, OnGltfAssetLoaded);
}

void UPlayerZeroLoaderComponent::ReplaceMeshWithGltfAsset(UglTFRuntimeAsset* GltfAsset)
{

	UE_LOG(LogTemp, Log, TEXT("AvatarLoader: Replacing mesh with GLTF asset."));
	
	if (!GltfAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarLoader: GLTF asset is null."));
		OnAvatarLoadComplete.Broadcast(nullptr);
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarLoader: Owner is null."));
		OnAvatarLoadComplete.Broadcast(nullptr);
		return;
	}



	// Remove old skeletal mesh if present
	if (TargetMeshComponent)
	{
		TargetMeshComponent->DestroyComponent();
		TargetMeshComponent = nullptr;
	}

	TArray<FglTFRuntimeNode> Nodes = GltfAsset->GetNodes();
	USkeletalMeshComponent* MasterMeshComponent = nullptr;

	// Build skeletal mesh config
	FglTFRuntimeSkeletalMeshConfig MeshConfig;
	if (!TargetSkeleton)
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarLoader: No TargetSkeleton set. Using default skeleton."));
		MeshConfig.Skeleton = TargetSkeleton;
		MeshConfig.SkeletonConfig.CopyRotationsFrom = TargetSkeleton;
	}

	//MeshConfig.bBuildPhysicsAsset = false;

	for (const FglTFRuntimeNode& Node : Nodes)
	{
		if (GltfAsset->NodeIsBone(Node.Index) || Node.Name.Contains("Armature"))
		{
			continue;
		}

		if (Node.SkinIndex >= 0)
		{
			USkeletalMesh* SkeletalMesh = GltfAsset->LoadSkeletalMesh(Node.MeshIndex, Node.SkinIndex, MeshConfig);
			if (!SkeletalMesh)
			{
				UE_LOG(LogTemp, Error, TEXT("AvatarLoader: Failed to load skeletal mesh from node %s"), *Node.Name);
				continue;
			}

			// Create new skeletal mesh component
			USkeletalMeshComponent* NewMeshComp = NewObject<USkeletalMeshComponent>(Owner);
			NewMeshComp->SetSkeletalMesh(SkeletalMesh);
			NewMeshComp->SetupAttachment(Owner->GetRootComponent());
			NewMeshComp->SetRelativeTransform(Node.Transform);
			NewMeshComp->RegisterComponent();

			// Make this component the master mesh
			if (!MasterMeshComponent)
			{
				MasterMeshComponent = NewMeshComp;
				TargetMeshComponent = NewMeshComp;

				// Set animation blueprint if needed
				if (TargetSkeleton && TargetMeshComponent && AnimationBlueprint)
				{
					TargetMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
					TargetMeshComponent->SetAnimClass(AnimationBlueprint);
				}
			}
			else
			{
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
				NewMeshComp->SetLeaderPoseComponent(MasterMeshComponent);
#else
				NewMeshComp->SetMasterPoseComponent(MasterMeshComponent);
#endif
			}
		}
	}

	if (TargetMeshComponent)
	{
		OnAvatarLoadComplete.Broadcast(TargetMeshComponent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarLoader: No skeletal mesh components were created."));
		OnAvatarLoadComplete.Broadcast(nullptr);
	}
}

