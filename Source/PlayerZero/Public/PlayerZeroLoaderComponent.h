#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
#include "PlayerZeroSubsystem.h"
#include "Animation/AnimInstance.h"
#include "Components/ActorComponent.h"
#include "PlayerZeroLoaderComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarLoadComplete, USkeletalMeshComponent*, LoadedMeshComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERZERO_API UPlayerZeroLoaderComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPlayerZeroLoaderComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader", meta=(AllowAbstract = "false"))
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	/** Avatar ID to load */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader")
	FString AvatarId;

	/** The mesh component to replace. If not set, will use the first USkeletalMeshComponent found on the Actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader")
	USkeletalMeshComponent* TargetMeshComponent;

	/** Skeleton to reuse (optional, but recommended) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader")
	USkeleton* TargetSkeleton;

	/** Called when the avatar is loaded and applied */
	UPROPERTY(BlueprintAssignable, Category = "Avatar Loader")
	FOnAvatarLoadComplete OnAvatarLoadComplete;

	UFUNCTION(BlueprintCallable, Category = "Avatar Loader")
	void LoadAvatar();

protected:
	virtual void BeginPlay() override;

	UPlayerZeroSubsystem* Subsystem;
	FOnGltfAssetLoaded OnGltfAssetLoaded;
private:
	UFUNCTION()
	void ReplaceMeshWithGltfAsset(UglTFRuntimeAsset* GltfAsset);
};
