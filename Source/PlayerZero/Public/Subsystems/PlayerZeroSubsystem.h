#pragma once

#include "glTFRuntimeAsset.h"
#include "PlayerZeroCharacterTypes.h"
#include "Api/GameEvents/GameEventApi.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerZeroSubsystem.generated.h"

struct FglTFRuntimeConfig;
struct FPlayerZeroCharacter;
struct FFileData;
struct FCharacterFindByIdResponse;
class FFileApi;
class FCharacterApi;


DECLARE_DELEGATE_OneParam(FOnCharacterDataLoaded, const FPlayerZeroCharacter&);
DECLARE_DELEGATE_OneParam(FOnAvatarDataDownloaded, const TArray<uint8>&);
DECLARE_DELEGATE_OneParam(FOnGltfAssetLoaded, UglTFRuntimeAsset*);
DECLARE_DELEGATE_OneParam(FOnAvatarTextureLoaded, UTexture2D*);

UCLASS()
class PLAYERZERO_API UPlayerZeroSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	FString GetHotLoadedAvatarId();
	
	void GetAvatarIconAsTexture(FString AvatarId, FOnAvatarTextureLoaded OnComplete);

	void GetAvatarMetaData(const FString& Id, FOnCharacterDataLoaded OnComplete);
	
	void DownloadAvatarData(const FString& Url, FOnAvatarDataDownloaded OnComplete);
	
	void LoadGltfAsset(const TArray<uint8>& Data, const FglTFRuntimeConfig& Config, FOnGltfAssetLoaded OnComplete);
	
	void LoadAvatarAsset(const FString& Id, const FOnGltfAssetLoaded& OnComplete);

private:

	UPROPERTY()
	FString CachedCharacterUrl;
	
	bool bIsInitialized = false;
	FString CachedAvatarId;

	// Add your persistent API instances, settings, etc.
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;

	void OnDeepLinkDataReceived(const FString& AvatarId);
};

