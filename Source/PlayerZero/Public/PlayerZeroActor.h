// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "glTFRuntimeAsset.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroLoaderComponent.h"
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
	TMap<FString, FPlayerZeroAnimationConfig> AnimationConfigsByBaseModelId;
=======
	/** Default constructor for ARpmActor. Initializes the actor and sets the root component. */
	ARpmActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Ready Player Me")
	FRpmAnimationConfig AnimationConfig;

	/** A map of animation configurations associated with base model IDs. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Ready Player Me")
	TMap<FString, FRpmAnimationConfig> AnimationConfigsByCharacterStyleId;
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmActor.h

	/** Configuration settings for loading static meshes from glTF assets. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Ready Player Me|Glb Import Settings")
	FglTFRuntimeStaticMeshConfig StaticMeshConfig;

	/** Configuration settings for loading skeletal meshes from glTF assets. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Ready Player Me|Glb Import Settings")
	FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;

<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroActor.h
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
=======
	/** Data related to the character being loaded, such as base model ID. */
	FRpmCharacterData CharacterData;

	/**
	 * @brief Loads a character using the provided character data and glTF asset.
	 * 
	 * This function sets the character data, retrieves animation configurations based on the base model ID, 
	 * and loads the skeletal mesh or static mesh components for the character.
	 * 
	 * @param InCharacterData Struct containing character-specific data like the base model ID.
	 * @param GltfAsset The glTF runtime asset used to load character mesh and skeleton.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void LoadCharacter(const FRpmCharacterData& InCharacterData, UglTFRuntimeAsset* GltfAsset);

	/**
	 * @brief Loads an asset (skeletal or static mesh) from a glTF asset.
	 * 
	 * @param Asset The asset to be loaded, including its type and ID.
	 * @param GltfAsset The glTF runtime asset to load the mesh from.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void LoadAsset(const FRpmAsset& Asset, UglTFRuntimeAsset* GltfAsset);

	/**
	 * @brief Loads a glTF asset and applies a skeleton configuration to the actor.
	 * 
	 * This function is used when loading a glTF asset with an associated skeleton and animation configuration.
	 * 
	 * @param GltfAsset The glTF asset containing the skeletal mesh.
	 * @param Asset The asset containing data related to the skeletal mesh to be loaded.
	 * @param InAnimationCharacter The animation configuration for the character.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void LoadGltfAssetWithSkeleton(UglTFRuntimeAsset* GltfAsset, const FRpmAsset& Asset, const FRpmAnimationConfig& InAnimationCharacter);

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
	/** The root component of the actor, used to attach mesh components. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Ready Player Me")
	USceneComponent* AssetRoot;

	/** A map storing arrays of mesh components, categorized by asset type (e.g., base model, clothing). */
	TMap<FString, TArray<USceneComponent*>> LoadedMeshComponentsByAssetType;

	/**
	 * @brief Loads mesh components from a glTF asset.
	 * 
	 * This function processes the nodes in a glTF asset and creates skeletal or static mesh components accordingly.
	 * 
	 * @param GltfAsset The glTF runtime asset containing the mesh data.
	 * @param AssetType The type of asset being loaded (e.g., base model or clothing).
	 * @return An array of scene components that were created from the glTF asset.
	 */
	TArray<USceneComponent*> LoadMeshComponents(UglTFRuntimeAsset* GltfAsset, const FString& AssetType);

	/**
	 * @brief Creates a skeletal mesh component based on the node in the glTF asset.
	 * 
	 * @param GltfAsset The glTF runtime asset containing the skeletal mesh.
	 * @param Node The node in the glTF asset that corresponds to the skeletal mesh.
	 * @return A pointer to the newly created skeletal mesh component.
	 */
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
