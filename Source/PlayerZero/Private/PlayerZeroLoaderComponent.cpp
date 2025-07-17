// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroLoaderComponent.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "PlayerZeroFunctionLibrary.h"
#include "Api/Assets/AssetApi.h"
#include "Api/Assets/Models/Asset.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterCreateResponse.h"
#include "Api/Characters/Models/CharacterFindByIdResponse.h"
#include "Api/Characters/Models/CharacterUpdateResponse.h"
#include "Api/Files/GlbLoader.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

UPlayerZeroLoaderComponent::UPlayerZeroLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	AppId = PlayerZeroSettings->ApplicationId;
	FileApi = MakeShared<FFileApi>();
	FileApi->OnAssetFileRequestComplete.BindUObject(this, &UPlayerZeroLoaderComponent::HandleAssetLoaded);
	FileApi->OnFileRequestComplete.BindUObject(this, &UPlayerZeroLoaderComponent::HandleCharacterAssetLoaded);
	CharacterApi = MakeShared<FCharacterApi>();
	CharacterApi->OnCharacterCreateResponse.BindUObject(this, &UPlayerZeroLoaderComponent::HandleCharacterCreateResponse);
	CharacterApi->OnCharacterUpdateResponse.BindUObject(this, &UPlayerZeroLoaderComponent::HandleCharacterUpdateResponse);
	CharacterApi->OnCharacterFindResponse.BindUObject(this, &UPlayerZeroLoaderComponent::HandleCharacterFindResponse);
	CharacterData = FPlayerZeroCharacterData();
	GltfConfig = FglTFRuntimeConfig();
	GltfConfig.TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
}

void UPlayerZeroLoaderComponent::SetGltfConfig(const FglTFRuntimeConfig* Config)
{
	GltfConfig = *Config;
}

void UPlayerZeroLoaderComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UPlayerZeroLoaderComponent::CreateCharacter(const FString& BaseModelId)
{
	CharacterData.BaseModelId = BaseModelId;
	FAsset BaseModelAsset = FAsset();
	BaseModelAsset.Id = BaseModelId;
	BaseModelAsset.Type = FAssetApi::BaseModelType;
	CharacterData.Assets.Add( FAssetApi::BaseModelType, BaseModelAsset);
	FCharacterCreateRequest CharacterCreateRequest = FCharacterCreateRequest();
	CharacterCreateRequest.Data.Assets = TMap<FString, FString>();
	CharacterCreateRequest.Data.Assets.Add(FAssetApi::BaseModelType, BaseModelId);
	CharacterCreateRequest.Data.ApplicationId = AppId;
	CharacterApi->CreateAsync(CharacterCreateRequest);
}

void UPlayerZeroLoaderComponent::LoadCharacterFromUrl(FString Url)
{
	FileApi->LoadFileFromUrl(Url);
}

void UPlayerZeroLoaderComponent::LoadAssetsFromCacheWithNewStyle()
{
	for (auto PreviewAssets : CharacterData.Assets)
	{
		if(PreviewAssets.Value.Type == FAssetApi::BaseModelType)
		{
			continue;
		}
		 LoadGltfRuntimeAssetFromCache(PreviewAssets.Value);
	}
}

void UPlayerZeroLoaderComponent::LoadAssetPreview(FAsset AssetData, bool bUseCache)
{
	if (CharacterData.BaseModelId.IsEmpty())
	{
		UE_LOG(LogPlayerZero, Error, TEXT("BaseModelId is empty"));
		return;
	}
	const bool bIsBaseModel = AssetData.Type == FAssetApi::BaseModelType;
	if(bIsBaseModel)
	{
		CharacterData.BaseModelId = AssetData.Id;
	}
	CharacterData.Assets.Add(AssetData.Type, AssetData);
	
	if(CharacterData.Id.IsEmpty())
	{
		LoadGltfRuntimeAssetFromCache(AssetData);
		if(bIsBaseModel && CharacterData.Assets.Num() > 1)
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

void UPlayerZeroLoaderComponent::HandleAssetLoaded(const TArray<unsigned char>* Data, const FAsset& Asset)
{
	if(!Data)
	{
		LoadGltfRuntimeAssetFromCache(Asset);
		return;
	}
	UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(*Data, GltfConfig);
	if(!GltfRuntimeAsset)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to load gltf asset"));
	}
	OnAssetLoaded.Broadcast(Asset, GltfRuntimeAsset);
}

void UPlayerZeroLoaderComponent::HandleCharacterAssetLoaded(const TArray<unsigned char>* Data, const FString& FileName)
{
	if(!Data)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to load character asset data"));
		return;
	}
	UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(*Data, GltfConfig);
	if(!GltfRuntimeAsset)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to load gltf asset"));
	}
	OnCharacterAssetLoaded.Broadcast(CharacterData, GltfRuntimeAsset);
}

void UPlayerZeroLoaderComponent::HandleCharacterCreateResponse(FCharacterCreateResponse CharacterCreateResponse, bool bWasSuccessful)
{
	if(bWasSuccessful && CharacterCreateResponse.IsValid())
	{
		CharacterData.Id = CharacterCreateResponse.Data.Id;
		OnCharacterCreated.Broadcast(CharacterData);
		LoadCharacterFromUrl(CharacterCreateResponse.Data.GlbUrl);
		return;
	}
	

	OnCharacterCreated.Broadcast(CharacterData);
	UE_LOG( LogPlayerZero, Error, TEXT("Failed to create character from web Api. Falling back to cache."));
	LoadCharacterAssetsFromCache(CharacterData.Assets);
}

void UPlayerZeroLoaderComponent::HandleCharacterUpdateResponse(FCharacterUpdateResponse CharacterUpdateResponse, bool bWasSuccessful)
{
	OnCharacterUpdated.Broadcast(CharacterData);
}

void UPlayerZeroLoaderComponent::HandleCharacterFindResponse(FCharacterFindByIdResponse CharacterFindByIdResponse, bool bWasSuccessful)
{
	OnCharacterFound.Broadcast(CharacterData);
}

void UPlayerZeroLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

