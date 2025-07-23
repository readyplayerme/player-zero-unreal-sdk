#include "PlayerZeroSubsystem.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "PlayerZero.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterFindByIdRequest.h"
#include "Api/Files/GlbLoader.h"

void UPlayerZeroSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FileApi = MakeShared<FFileApi>();
	CharacterApi = MakeShared<FCharacterApi>();
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

void UPlayerZeroSubsystem::LoadAvatarById(const FString& Id, const FOnGltfLoaded& OnComplete)
{
	// Step 1: Fetch character metadata
	TSharedPtr<FCharacterFindByIdRequest> Request = MakeShared<FCharacterFindByIdRequest>();
	Request->Id = Id;

	CharacterApi->FindByIdAsync(*Request, FOnCharacterFindResponse::CreateLambda(
		[this, OnComplete](FCharacterFindByIdResponse Response, bool bCharacterFetchSuccessful)
		{
			if (!bCharacterFetchSuccessful || Response.Data.Id.IsEmpty())
			{
				UE_LOG(LogPlayerZero, Error, TEXT("Failed to find character or ID was empty"));
				OnComplete.ExecuteIfBound(nullptr);
				return;
			}

			const FString GlbUrl = Response.Data.ModelUrl;
			UE_LOG( LogPlayerZero, Log, TEXT("Character GLB URL: %s"), *GlbUrl);
			if (GlbUrl.IsEmpty())
			{
				UE_LOG(LogPlayerZero, Error, TEXT("Character GLB URL is empty"));
				OnComplete.ExecuteIfBound(nullptr);
				return;
			}
			
			FileApi->LoadFileFromUrl(GlbUrl, FOnAssetFileRequestComplete::CreateLambda(
				[this, OnComplete](const FFileData& File, const TArray<uint8>& Data)
				{
					if (Data.Num() == 0)
					{
						UE_LOG(LogPlayerZero, Error, TEXT("Downloaded GLB data is empty"));
						OnComplete.ExecuteIfBound(nullptr);
						return;
					}
					UE_LOG(LogPlayerZero, Log, TEXT("Successfully downloaded GLB data of size: %d bytes"), Data.Num());
					// Step 3: Load the GLTF asset
					FglTFRuntimeConfig Config;
					UglTFRuntimeAsset* GltfAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, Config);
					
					if (!GltfAsset)
					{
						UE_LOG(LogPlayerZero, Error, TEXT("Failed to load GLTF asset from data"));
					}
					UE_LOG(LogPlayerZero, Log, TEXT("Successfully loaded GLTF asset "));
					OnComplete.ExecuteIfBound(GltfAsset);
				}));
		}));
}



void UPlayerZeroSubsystem::FindCharacterById(const FString& Id, FOnCharacterDataLoaded OnComplete)
{
	TSharedPtr<FCharacterFindByIdRequest> Request = MakeShared<FCharacterFindByIdRequest>();
	Request->Id = Id;

	CharacterApi->FindByIdAsync(*Request, FOnCharacterFindResponse::CreateLambda(
		[OnComplete](FCharacterFindByIdResponse Response, bool bWasSuccessful)
		{
			if (bWasSuccessful)
			{
				OnComplete.ExecuteIfBound(Response.Data);
			}
			else
			{
				OnComplete.ExecuteIfBound(FPlayerZeroCharacter());
			}
		}));
}


void UPlayerZeroSubsystem::DownloadAvatar(const FString& Url, FOnAvatarDownloaded OnComplete)
{
	FileApi->LoadFileFromUrl(Url, FOnAssetFileRequestComplete::CreateLambda(
		[OnComplete](const FFileData& File, const TArray<uint8>& Data)
		{
			OnComplete.ExecuteIfBound(Data);
		}));
}


void UPlayerZeroSubsystem::LoadGltf(const TArray<uint8>& Data, const FglTFRuntimeConfig& Config, FOnGltfLoaded OnComplete)
{
	UglTFRuntimeAsset* Asset = nullptr;
	if (Data.Num() > 0)
	{
		Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, Config);
	}
	else
	{
		UE_LOG(LogPlayerZero, Error, TEXT("GLTF data is empty."));
	}
	OnComplete.ExecuteIfBound(Asset);
}

void UPlayerZeroSubsystem::OnDeepLinkDataReceived(const FString& AvatarId)
{
}
