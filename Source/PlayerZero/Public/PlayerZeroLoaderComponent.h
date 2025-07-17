// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroLoaderComponent.h
#include "Api/Assets/Models/Asset.h"
#include "Api/Characters/Models/PlayerZeroCharacter.h"
=======
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Characters/Models/RpmCharacter.h"
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmLoaderComponent.h
#include "Components/ActorComponent.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroLoaderComponent.generated.h"

class FAssetApi;
class FFileApi;
class FGlbLoader;
struct FCharacterCreateResponse;
struct FCharacterUpdateResponse;
struct FCharacterFindByIdResponse;
class FCharacterApi;
struct FRpmAsset;

<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroLoaderComponent.h
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreated, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterUpdated, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFound, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAssetLoaded, const FPlayerZeroCharacterData&, CharacterData, UglTFRuntimeAsset*, GltfRuntimeAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAssetLoaded, const FAsset&, Asset, UglTFRuntimeAsset*, GltfRuntimeAsset );
=======
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreated, FRpmCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterUpdated, FRpmCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFound, FRpmCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAssetLoaded, const FRpmCharacterData&, CharacterData, UglTFRuntimeAsset*, GltfRuntimeAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAssetLoaded, const FRpmAsset&, Asset, UglTFRuntimeAsset*, GltfRuntimeAsset );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetRemoved, const FRpmAsset&, Asset);
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmLoaderComponent.h

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
	UPROPERTY(BlueprintAssignable, Category = "Ready Player Me" )
	FOnAssetRemoved OnAssetRemoved;

	UPlayerZeroLoaderComponent();
	
	void SetGltfConfig(const FglTFRuntimeConfig* Config);

	void HandleAssetLoaded(const TArray<uint8>& Data, const FRpmAsset& Asset);
	void HandleCharacterAssetLoaded(const TArray<uint8>& Array, const FString& FileName);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	FglTFRuntimeConfig GltfConfig;
	FString AppId;
	FPlayerZeroCharacter Character;
	FPlayerZeroCharacterData CharacterData;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void CreateCharacterFromFirstStyle();
	
<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroLoaderComponent.h
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void CreateCharacter(const FString& BaseModelId);
=======
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void CreateCharacter(const FString& CharacterStyleId);

	virtual void UpdateCharacter(const TMap<FString, FString>& Assets);

	virtual void FindCharacterById(const FString CharacterId);
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmLoaderComponent.h

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacterFromUrl(FString Url);
	
<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroLoaderComponent.h
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	void LoadGltfRuntimeAssetFromCache(const FAsset& Asset);

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacterAssetsFromCache(TMap<FString, FAsset> AssetMap);
	
	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadAssetPreview(FAsset AssetData, bool bUseCache);
=======
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void LoadGltfRuntimeAssetFromCache(const FRpmAsset& Asset);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void LoadCharacterAssetsFromCache(TMap<FString, FRpmAsset> AssetMap);
	
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void LoadAssetPreview(FRpmAsset AssetData);
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmLoaderComponent.h
	
	virtual void HandleCharacterCreateResponse(TSharedPtr<FCharacterCreateResponse> Response, bool bWasSuccessful);
	virtual void HandleCharacterUpdateResponse(TSharedPtr<FCharacterUpdateResponse> CharacterUpdateResponse, bool bWasSuccessful);
	virtual void HandleCharacterFindResponse(TSharedPtr<FCharacterFindByIdResponse> CharacterFindByIdResponse, bool bWasSuccessful);
	
private:
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
	TSharedPtr<FAssetApi> AssetApi;
	void LoadAssetsFromCacheWithNewStyle();
};
