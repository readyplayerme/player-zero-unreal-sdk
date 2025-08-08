// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "glTFRuntimeAsset.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroActor.generated.h"

/**
 * @class ARpmActor
 * @brief An actor class used to load and manage characters using glTF assets in the Ready Player Me system.
 * 
 * This class provides functionality to load skeletal and static meshes from glTF assets, 
 * apply animations, and manage different mesh components dynamically. 
 */
UCLASS()
class PLAYERZERO_API APlayerZeroActor : public AActor
{
	GENERATED_BODY()
	
public:	
<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroActor.h
	APlayerZeroActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player Zero")
	FPlayerZeroAnimationConfig AnimationConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player Zero")
	TMap<FString, FPlayerZeroAnimationConfig> AnimationConfigsByStyleId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player Zero|Glb Import Settings")
	FglTFRuntimeStaticMeshConfig StaticMeshConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player Zero|Glb Import Settings")
	FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;

<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroActor.h
	FPlayerZeroCharacterData CharacterData;
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacter(const FPlayerZeroCharacterData& InCharacterData, UglTFRuntimeAsset* GltfAsset);
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	void RemoveAllMeshes();
	
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Removes all mesh components attached to the actor.
	 * 
	 * This function clears all loaded mesh components, including both static and skeletal meshes.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void RemoveAllMeshes();

	/**
	 * @brief Removes mesh components based on their asset type.
	 * 
	 * This function removes specific mesh components based on their type (e.g., base model or clothing).
	 * 
	 * @param AssetType The type of the asset (e.g., "Custom top") to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmActor.h
	void RemoveMeshComponentsOfType(const FString& AssetType);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void RemoveAssetOfType(const FRpmAsset& Asset );

protected:
	/** Weak reference to the master pose component, used for synchronizing skeletal animations. */
	TWeakObjectPtr<USkeletalMeshComponent> MasterPoseComponent;

	/** Called when the actor starts or is spawned. Initializes the actor's state. */
	virtual void BeginPlay() override;

	/**
	 * @brief Generates a unique name for a node in the glTF asset.
	 * 
	 * @tparam T The type of component (e.g., skeletal or static mesh) for which to generate a unique name.
	 * @param Node The node from the glTF asset that will be assigned a unique name.
	 * @return A unique name for the node.
	 */
	template<typename T>
	FName GetSafeNodeName(const FglTFRuntimeNode& Node)
	{
		return MakeUniqueObjectName(this, T::StaticClass(), *Node.Name);
	}
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Player Zero")
	USceneComponent* AssetRoot;

	TArray<USceneComponent*> LoadedMeshComponents;
	
	TArray<USceneComponent*> LoadMeshComponents(UglTFRuntimeAsset* GltfAsset);
	USkeletalMeshComponent* CreateSkeletalMeshComponent(UglTFRuntimeAsset* GltfAsset, const FglTFRuntimeNode& Node);

	/**
	 * @brief Creates a static mesh component based on the node in the glTF asset.
	 * 
	 * @param GltfAsset The glTF runtime asset containing the static mesh.
	 * @param Node The node in the glTF asset that corresponds to the static mesh.
	 * @return A pointer to the newly created static mesh component.
	 */
	UStaticMeshComponent* CreateStaticMeshComponent(UglTFRuntimeAsset* GltfAsset, const FglTFRuntimeNode& Node);

	/**
	 * @brief Removes mesh components from the actor.
	 * 
	 * @param Components The array of components to be removed.
	 */
	void RemoveMeshComponents(const TArray<USceneComponent*>& Components);
};
