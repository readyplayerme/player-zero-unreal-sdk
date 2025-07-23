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

	TSharedPtr<FCharacterFindByIdRequest> Request = MakeShared<FCharacterFindByIdRequest>();
	Request->Id = Id;
	//
	// CharacterApi->FindByIdAsync(*Request, FOnCharacterFindResponse::CreateLambda(
	// 	[OnComplete](FCharacterFindByIdResponse Response, bool bWasSuccessful)
	// 	{
	// 		if (bWasSuccessful)
	// 		{
	// 			OnComplete.ExecuteIfBound(Response.Data);
	// 		}
	// 		else
	// 		{
	// 			OnComplete.ExecuteIfBound(FPlayerZeroCharacter());
	// 		}
	// 	}));

	// FileApi->LoadFileFromUrl(Url, FOnAssetFileRequestComplete::CreateLambda(
	// [OnComplete](const FFileData& File, const TArray<uint8>& Data)
	// {
	// 	OnComplete.ExecuteIfBound(Data);
	// }));

	// UglTFRuntimeAsset* Asset = nullptr;
	// if (Data.Num() > 0)
	// {
	// 	Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, Config);
	// }
	// else
	// {
	// 	UE_LOG(LogPlayerZero, Error, TEXT("GLTF data is empty."));
	// }
	// OnComplete.ExecuteIfBound(Asset);
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
