#pragma once

#include "PlayerZeroCharacterTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerZeroSubsystem.generated.h"

struct FFileData;
struct FCharacterFindByIdResponse;
class UglTFRuntimeAsset;
class FFileApi;
class FCharacterApi;


DECLARE_DELEGATE_OneParam(FOnCharacterDataLoaded, const FPlayerZeroCharacterData&);
DECLARE_DELEGATE_OneParam(FOnAvatarDownloaded, const TArray<uint8>&);
DECLARE_DELEGATE_OneParam(FOnGltfLoaded, UglTFRuntimeAsset*);
DECLARE_DELEGATE_OneParam(FOnAvatarSpawned, AActor*);

UCLASS()
class PLAYERZERO_API UPlayerZeroSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	FString GetHotLoadedAvatarId();

	void GetAvatarMetaData(const FString& AvatarId) const;
	
	void LoadAvatarById(const FString& Id, FOnGltfLoaded OnComplete);
    
	void FindCharacterById(const FString& Id, FOnCharacterDataLoaded OnComplete);
	void DownloadAvatar(const FString& Url, FOnAvatarDownloaded OnComplete);
	void LoadGltf(const TArray<uint8>& BinaryData, FOnGltfLoaded OnComplete);
	void SpawnActorFromGltf(UglTFRuntimeAsset* Asset, const FPlayerZeroCharacterData& Meta, FOnAvatarSpawned OnComplete);
	void HandleCharacterFindResponse(FCharacterFindByIdResponse CharacterFindByIdResponse, bool bWasSuccessful);


	void HandleCharacterAssetLoaded(FFileData& File, const TArray<unsigned char>& Array);

	// More API methods...

protected:
    
private:
	bool bIsInitialized = false;
	FString CachedAvatarId;

	// Add your persistent API instances, settings, etc.
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
	//TSharedPtr<FGameEventApi> GameEventApi;

	void OnDeepLinkDataReceived(const FString& AvatarId);
};

