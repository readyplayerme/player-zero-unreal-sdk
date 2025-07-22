#include "PlayerZeroSubsystem.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterFindByIdRequest.h"
#include "Api/Files/GlbLoader.h"

void UPlayerZeroSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FileApi = MakeShared<FFileApi>();
	FileApi->OnAssetFileRequestComplete.BindUObject(this, &UPlayerZeroSubsystem::HandleCharacterAssetLoaded);
	CharacterApi = MakeShared<FCharacterApi>();
	CharacterApi->OnCharacterFindResponse.BindUObject(this, &UPlayerZeroSubsystem::HandleCharacterFindResponse);
}

void UPlayerZeroSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FString UPlayerZeroSubsystem::GetHotLoadedAvatarId()
{
	if (bIsInitialized)
	{
		return CachedAvatarId;
	}
	
	// Initialize the SDK and set the CachedAvatarId
	bIsInitialized = true;
	CachedAvatarId = TEXT("default_avatar_id"); // Replace with actual logic to get the avatar ID
	return CachedAvatarId;
}

void UPlayerZeroSubsystem::GetAvatarMetaData(const FString& AvatarId) const
{
	TSharedPtr<FCharacterFindByIdRequest> CharacterFindByIdRequest = MakeShared<FCharacterFindByIdRequest>();
	CharacterFindByIdRequest->Id = AvatarId;
}

void UPlayerZeroSubsystem::LoadAvatarById(const FString& Id, FOnGltfLoaded OnComplete)
{
	FindCharacterById(Id, FOnCharacterDataLoaded::CreateLambda(
		[this, OnComplete](const FPlayerZeroCharacterData& Character)
		{
			FString Url = Character.Id;

			DownloadAvatar(Url, FOnAvatarDownloaded::CreateLambda(
				[this, OnComplete](const TArray<uint8>& Data)
				{
					FglTFRuntimeConfig GltfConfig = FglTFRuntimeConfig();
					UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, GltfConfig);
					OnComplete.ExecuteIfBound(GltfRuntimeAsset); // Replace nullptr with the actual actor spawned from the GLTF data
				}));
		}));
}

void UPlayerZeroSubsystem::FindCharacterById(const FString& Id, FOnCharacterDataLoaded OnComplete)
{
	TSharedPtr<FCharacterFindByIdRequest> CharacterFindByIdRequest = MakeShared<FCharacterFindByIdRequest>();
	CharacterFindByIdRequest->Id = Id;

	CharacterApi->FindByIdAsync(*CharacterFindByIdRequest);
	OnComplete.ExecuteIfBound(FPlayerZeroCharacterData()); // Replace with actual logic to get character data
}

void UPlayerZeroSubsystem::DownloadAvatar(const FString& Url, FOnAvatarDownloaded OnComplete)
{
}

void UPlayerZeroSubsystem::LoadGltf(const TArray<uint8>& BinaryData, FOnGltfLoaded OnComplete)
{
}

void UPlayerZeroSubsystem::SpawnActorFromGltf(UglTFRuntimeAsset* Asset, const FPlayerZeroCharacterData& Meta,
	FOnAvatarSpawned OnComplete)
{
}

void UPlayerZeroSubsystem::HandleCharacterFindResponse(FCharacterFindByIdResponse CharacterFindByIdResponse,
                                                       bool bWasSuccessful)
{
}

void UPlayerZeroSubsystem::HandleCharacterAssetLoaded(FFileData& File,
	const TArray<unsigned char>& Array)
{
}

void UPlayerZeroSubsystem::OnDeepLinkDataReceived(const FString& AvatarId)
{
}
