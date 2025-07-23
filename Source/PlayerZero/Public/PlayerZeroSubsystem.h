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
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAvatarDownloaded, const TArray<uint8>&, FileData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGltfLoaded, UglTFRuntimeAsset*, GltfAsset);
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
	void GetAvatarMetaData(const FString& AvatarId) const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void FindCharacterById(const FString& Id, FOnCharacterDataLoaded OnComplete);

	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void DownloadAvatar(const FString& Url, FOnAvatarDownloaded OnComplete);

	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void LoadGltf(const TArray<uint8>& Data, const FglTFRuntimeConfig& Config, FOnGltfLoaded OnComplete);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	void LoadAvatarById(const FString& Id, const FOnGltfLoaded& OnComplete);

private:
	// Used for callback chaining internally
	UPROPERTY()
	FOnGltfLoaded OnAvatarLoadedDelegate;

	UPROPERTY()
	FString CachedCharacterUrl;
	
	bool bIsInitialized = false;
	FString CachedAvatarId;

	// Add your persistent API instances, settings, etc.
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
	//TSharedPtr<FGameEventApi> GameEventApi;

	void OnDeepLinkDataReceived(const FString& AvatarId);

	UPROPERTY()
	FOnGltfLoaded OnAvatarLoadComplete;

};

