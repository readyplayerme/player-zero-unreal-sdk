// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "glTFRuntimeAsset.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroLoaderComponent.h"
#include "PlayerZeroActor.generated.h"

UCLASS()
class PLAYERZERO_API APlayerZeroActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerZeroActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player Zero")
	FPlayerZeroAnimationConfig AnimationConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player Zero")
	TMap<FString, FPlayerZeroAnimationConfig> AnimationConfigsByBaseModelId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Ready Player Me|Glb Import Settings")
	FglTFRuntimeStaticMeshConfig StaticMeshConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Ready Player Me|Glb Import Settings")
	FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;

	FPlayerZeroCharacterData CharacterData;
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacter(const FPlayerZeroCharacterData& InCharacterData, UglTFRuntimeAsset* GltfAsset);

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadAsset(const FAsset& Asset, UglTFRuntimeAsset* GltfAsset );
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadGltfAssetWithSkeleton(UglTFRuntimeAsset* GltfAsset, const FAsset& Asset, const FPlayerZeroAnimationConfig& InAnimationCharacter);

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	void RemoveAllMeshes();
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	void RemoveMeshComponentsOfType(const FString& AssetType);
	
	virtual void Tick(float DeltaTime) override;

protected:
	TWeakObjectPtr<USkeletalMeshComponent> MasterPoseComponent;
	
	virtual void BeginPlay() override;

	template<typename T>
	FName GetSafeNodeName(const FglTFRuntimeNode& Node)
	{
		return MakeUniqueObjectName(this, T::StaticClass(), *Node.Name);
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Ready Player Me")
	USceneComponent* AssetRoot;

	TMap<FString, TArray<USceneComponent*>> LoadedMeshComponentsByAssetType;
	
	TArray<USceneComponent*> LoadMeshComponents(UglTFRuntimeAsset* GltfAsset, const FString& AssetType);
	USkeletalMeshComponent* CreateSkeletalMeshComponent(UglTFRuntimeAsset* GltfAsset, const FglTFRuntimeNode& Node);
	UStaticMeshComponent* CreateStaticMeshComponent(UglTFRuntimeAsset* GltfAsset, const FglTFRuntimeNode& Node);

};
