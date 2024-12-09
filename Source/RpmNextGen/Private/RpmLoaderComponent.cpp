// Copyright Ready Player Me


#include "RpmLoaderComponent.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "RpmFunctionLibrary.h"
#include "Api/Assets/AssetApi.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterCreateResponse.h"
#include "Api/Characters/Models/CharacterFindByIdResponse.h"
#include "Api/Characters/Models/CharacterUpdateResponse.h"
#include "Api/Files/GlbLoader.h"
#include "Cache/AssetCacheManager.h"
#include "Settings/RpmDeveloperSettings.h"

URpmLoaderComponent::URpmLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	AppId = RpmSettings->ApplicationId;
	FileApi = MakeShared<FFileApi>();
	FileApi->OnAssetFileRequestComplete.BindUObject(this, &URpmLoaderComponent::HandleAssetLoaded);
	FileApi->OnFileRequestComplete.BindUObject(this, &URpmLoaderComponent::HandleCharacterAssetLoaded);
	AssetApi = MakeShared<FAssetApi>();
	CharacterApi = MakeShared<FCharacterApi>();
	CharacterData = FRpmCharacterData();
	GltfConfig = FglTFRuntimeConfig();
	GltfConfig.TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
}

void URpmLoaderComponent::SetGltfConfig(const FglTFRuntimeConfig* Config)
{
	GltfConfig = *Config;
}

void URpmLoaderComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void URpmLoaderComponent::CreateCharacterFromFirstStyle()
{
	TSharedPtr<FAssetListRequest> AssetListRequest = MakeShared<FAssetListRequest>(FAssetListQueryParams());
	AssetListRequest->Params.ApplicationId = AppId;
	AssetListRequest->Params.Limit = 1;
	AssetListRequest->Params.Type = FAssetApi::CharacterStyleAssetType;
	TWeakObjectPtr<URpmLoaderComponent> WeakAssetApi = TWeakObjectPtr<URpmLoaderComponent>(this);
	AssetApi->ListAssetsAsync(AssetListRequest, FOnListAssetsResponse::CreateLambda( [WeakAssetApi](TSharedPtr<FAssetListResponse> Response, bool bWasSuccessful)
	{
		if (WeakAssetApi.IsValid())
		{
			if(bWasSuccessful && Response.IsValid() && Response->Data.Num() > 0)
			{
				WeakAssetApi->CreateCharacter(Response->Data[0].Id);
				return;
			}
			UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to fetch first asset id"));
		}
	}));
}

void URpmLoaderComponent::CreateCharacter(const FString& CharacterStyleId)
{
	CharacterData.CharacterStyleId = CharacterStyleId;
	FRpmAsset StyleAsset = FRpmAsset();
	StyleAsset.Id = CharacterStyleId;
	StyleAsset.Type = FAssetApi::CharacterStyleAssetType;
	CharacterData.Assets.Add( FAssetApi::CharacterStyleAssetType, StyleAsset);
	TSharedPtr<FCharacterCreateRequest> CharacterCreateRequest = MakeShared<FCharacterCreateRequest>();
	CharacterCreateRequest->Data.Assets = TMap<FString, FString>();
	CharacterCreateRequest->Data.Assets.Add(FAssetApi::CharacterStyleAssetType, CharacterStyleId);
	CharacterCreateRequest->Data.ApplicationId = AppId;
	CharacterApi->CreateAsync(CharacterCreateRequest, FOnCharacterCreateResponse::CreateUObject(this, &URpmLoaderComponent::HandleCharacterCreateResponse));
}

void URpmLoaderComponent::UpdateCharacter(const TMap<FString, FString>& Assets)
{
	TSharedPtr<FCharacterUpdateRequest> CharacterCreateRequest = MakeShared<FCharacterUpdateRequest>();
	CharacterCreateRequest->Payload.Assets = Assets;
	CharacterApi->UpdateAsync(CharacterCreateRequest , FOnCharacterUpdatResponse::CreateUObject(this, &URpmLoaderComponent::HandleCharacterUpdateResponse));
}

void URpmLoaderComponent::FindCharacterById(const FString CharacterId)
{
	TSharedPtr<FCharacterFindByIdRequest> CharacterFindByIdRequest = MakeShared<FCharacterFindByIdRequest>();
	CharacterFindByIdRequest->Id = CharacterId;
	CharacterApi->FindByIdAsync(CharacterFindByIdRequest, FOnCharacterFindResponse::CreateUObject(this, &URpmLoaderComponent::HandleCharacterFindResponse));
}

void URpmLoaderComponent::LoadCharacterFromUrl(FString Url)
{
	FileApi->LoadFileFromUrl(Url);
}

void URpmLoaderComponent::LoadGltfRuntimeAssetFromCache(const FRpmAsset& Asset)
{
	FCachedAssetData ExistingAsset;
	if(FAssetCacheManager::Get().GetCachedAsset(Asset.Id, ExistingAsset))
	{
		CharacterData.Assets.Add(ExistingAsset.Type, Asset);
		TArray<uint8> Data;
		if(FFileApi::LoadFileFromPath(ExistingAsset.GetGlbPathForCharacterStyleId(CharacterData.CharacterStyleId), Data))
		{
			UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, GltfConfig);
			if(!GltfRuntimeAsset)
			{
				UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load gltf asset"));
			}
			OnAssetLoaded.Broadcast(Asset, GltfRuntimeAsset);
			return;
		}
	}
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load gltf asset from cache"));
	OnAssetLoaded.Broadcast(Asset, nullptr);
}

void URpmLoaderComponent::LoadCharacterAssetsFromCache(TMap<FString, FRpmAsset> AssetMap)
{
	for (auto Element : AssetMap)
	{
		LoadGltfRuntimeAssetFromCache(Element.Value);
	}
}

void URpmLoaderComponent::LoadAssetsFromCacheWithNewStyle()
{
	for (auto PreviewAssets : CharacterData.Assets)
	{
		if(PreviewAssets.Value.Type == FAssetApi::CharacterStyleAssetType)
		{
			continue;
		}
		 LoadGltfRuntimeAssetFromCache(PreviewAssets.Value);
	}
}

void URpmLoaderComponent::LoadAssetPreview(FRpmAsset AssetData)
{
	if (CharacterData.CharacterStyleId.IsEmpty())
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("CharacterStyleId is empty"));
		return;
	}
	const bool bIsStyleAsset = AssetData.Type == FAssetApi::CharacterStyleAssetType;
	if(bIsStyleAsset)
	{
		CharacterData.CharacterStyleId = AssetData.Id;
	}
	bool bShouldRemoveAsset = !bIsStyleAsset && CharacterData.Assets.Contains(AssetData.Type) && CharacterData.Assets[AssetData.Type].Id == AssetData.Id;

	if(bShouldRemoveAsset)
	{
		CharacterData.Assets.Remove(AssetData.Type);
	}
	else
	{
		CharacterData.Assets.Add(AssetData.Type, AssetData);
	}
	// character created from cache
	if(CharacterData.Id.IsEmpty())
	{
		// if asset needs to be removed just broadcast the event and return
		if(bShouldRemoveAsset)
		{
			OnAssetRemoved.Broadcast(AssetData);
			return;
		}
		LoadGltfRuntimeAssetFromCache(AssetData);
		if(bIsStyleAsset && CharacterData.Assets.Num() > 1)
		{
			LoadAssetsFromCacheWithNewStyle();
		}
		return;
	}
	TMap<FString, FString> ParamAssets;
	for (auto Asset : CharacterData.Assets)
	{
		ParamAssets.Add(Asset.Key, Asset.Value.Id);
	}
	FCharacterPreviewRequest PreviewRequest;
	PreviewRequest.Id = CharacterData.Id;
	PreviewRequest.Params.Assets = ParamAssets;
	const FString& Url = CharacterApi->GeneratePreviewUrl(PreviewRequest);

	FileApi->LoadFileFromUrl(Url);
}

void URpmLoaderComponent::HandleAssetLoaded(const TArray<uint8>& Data, const FRpmAsset& Asset)
{
	if(Data.Num() == 0)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Invalid or empty data received for asset: %s. Trying to load from cache."), *Asset.Id);
		LoadGltfRuntimeAssetFromCache(Asset);
		return;
	}
	UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, GltfConfig);
	if(!GltfRuntimeAsset)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load gltf asset."));
	}
	OnAssetLoaded.Broadcast(Asset, GltfRuntimeAsset);
}

void URpmLoaderComponent::HandleCharacterAssetLoaded(const TArray<uint8>& Data, const FString& FileName)
{
	if(Data.Num() == 0)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load character asset data"));
		return;
	}
	UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, GltfConfig);
	if(!GltfRuntimeAsset)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load gltf asset"));
	}
	OnCharacterAssetLoaded.Broadcast(CharacterData, GltfRuntimeAsset);
}

void URpmLoaderComponent::HandleCharacterCreateResponse(TSharedPtr<FCharacterCreateResponse> Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response.IsValid())
	{
		CharacterData.Id = Response->Data.Id;
		OnCharacterCreated.Broadcast(CharacterData);
		LoadCharacterFromUrl(Response->Data.GlbUrl);
		return;
	}
	

	OnCharacterCreated.Broadcast(CharacterData);
	UE_LOG( LogReadyPlayerMe, Error, TEXT("Failed to create character from web Api. Falling back to cache."));
	LoadCharacterAssetsFromCache(CharacterData.Assets);
}

void URpmLoaderComponent::HandleCharacterUpdateResponse(TSharedPtr<FCharacterUpdateResponse> CharacterUpdateResponse, bool bWasSuccessful)
{
	OnCharacterUpdated.Broadcast(CharacterData);
}

void URpmLoaderComponent::HandleCharacterFindResponse(TSharedPtr<FCharacterFindByIdResponse> CharacterFindByIdResponse, bool bWasSuccessful)
{
	OnCharacterFound.Broadcast(CharacterData);
}

void URpmLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

