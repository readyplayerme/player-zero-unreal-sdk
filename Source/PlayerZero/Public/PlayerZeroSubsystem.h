#pragma once

#include "PlayerZeroCharacterTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerZeroSubsystem.generated.h"

struct FglTFRuntimeConfig;
struct FPlayerZeroCharacter;
struct FFileData;
struct FCharacterFindByIdResponse;
class FFileApi;
class FCharacterApi;


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharacterDataLoaded, const FPlayerZeroCharacter&, Character);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAvatarDataDownloaded, const TArray<uint8>&, FileData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGltfAssetLoaded, UglTFRuntimeAsset*, GltfAsset);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAvatarSpawned, AActor*, SpawnedActor);

UCLASS()
class PLAYERZERO_API UPlayerZeroSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	FString GetHotLoadedAvatarId();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void GetAvatarMetaData(const FString& Id, FOnCharacterDataLoaded OnComplete);

	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void DownloadAvatarData(const FString& Url, FOnAvatarDataDownloaded OnComplete);

	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void LoadGltfAsset(const TArray<uint8>& Data, const FglTFRuntimeConfig& Config, FOnGltfAssetLoaded OnComplete);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void LoadAvatarAsset(const FString& Id, const FOnGltfAssetLoaded& OnComplete);

private:

	UPROPERTY()
	FString CachedCharacterUrl;
	
	bool bIsInitialized = false;
	FString CachedAvatarId;

	// Add your persistent API instances, settings, etc.
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
	//TSharedPtr<FGameEventApi> GameEventApi;

	void OnDeepLinkDataReceived(const FString& AvatarId);
};

