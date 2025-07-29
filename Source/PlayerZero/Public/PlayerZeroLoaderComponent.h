#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
#include "Animation/AnimInstance.h"
#include "Components/ActorComponent.h"
#include "PlayerZeroLoaderComponent.generated.h"

struct FCharacterConfig;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarLoadComplete, USkeletalMeshComponent*, LoadedMeshComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERZERO_API UPlayerZeroLoaderComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPlayerZeroLoaderComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader", meta=(AllowAbstract = "false"))
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader")
	FString AvatarId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader")
	USkeletalMeshComponent* TargetMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Loader")
	USkeleton* TargetSkeleton;

	UPROPERTY(BlueprintAssignable, Category = "Avatar Loader")
	FOnAvatarLoadComplete OnAvatarLoadComplete;

	UFUNCTION(BlueprintCallable, Category = "Avatar Loader")
	void LoadAvatar( const FCharacterConfig& Config);
	
	void LogSkeletonCompatibility(USkeletalMesh* Mesh, USkeleton* ExpectedSkeleton);
	
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void ReplaceMeshWithGltfAsset(UglTFRuntimeAsset* GltfAsset);
};
