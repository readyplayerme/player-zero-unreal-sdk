// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
#include "Api/Assets/Models/Asset.h"
#include "Api/Characters/Models/PlayerZeroCharacter.h"
#include "Components/ActorComponent.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroLoaderComponent.generated.h"

class FFileApi;
class FGlbLoader;
struct FCharacterCreateResponse;
struct FCharacterUpdateResponse;
struct FCharacterFindByIdResponse;
class FCharacterApi;
struct FAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreated, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterUpdated, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFound, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAssetLoaded, const FPlayerZeroCharacterData&, CharacterData, UglTFRuntimeAsset*, GltfRuntimeAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAssetLoaded, const FAsset&, Asset, UglTFRuntimeAsset*, GltfRuntimeAsset );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERZERO_API UPlayerZeroLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnCharacterAssetLoaded OnCharacterAssetLoaded;
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnAssetLoaded OnAssetLoaded;
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnCharacterCreated OnCharacterCreated;
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnCharacterUpdated OnCharacterUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnCharacterFound OnCharacterFound;

	UPlayerZeroLoaderComponent();
	
	void SetGltfConfig(const FglTFRuntimeConfig* Config);

	void HandleAssetLoaded(const TArray<unsigned char>* Data, const FAsset& Asset);
	void HandleCharacterAssetLoaded(const TArray<unsigned char>* Array, const FString& FileName);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	FglTFRuntimeConfig GltfConfig;
	FString AppId;
	FPlayerZeroCharacter Character;
	FPlayerZeroCharacterData CharacterData;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void CreateCharacter(const FString& BaseModelId);

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacterFromUrl(FString Url);
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	void LoadGltfRuntimeAssetFromCache(const FAsset& Asset);

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacterAssetsFromCache(TMap<FString, FAsset> AssetMap);
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadAssetPreview(FAsset AssetData, bool bUseCache);
	
	UFUNCTION()
	virtual void HandleCharacterCreateResponse(FCharacterCreateResponse CharacterCreateResponse, bool bWasSuccessful);
	UFUNCTION()
	virtual void HandleCharacterUpdateResponse(FCharacterUpdateResponse CharacterUpdateResponse, bool bWasSuccessful);
	UFUNCTION()
	virtual void HandleCharacterFindResponse(FCharacterFindByIdResponse CharacterFindByIdResponse, bool bWasSuccessful);
	
private:
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;

	void LoadAssetsFromCacheWithNewStyle();
};
