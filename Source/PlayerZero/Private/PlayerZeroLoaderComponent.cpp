// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroLoaderComponent.h"
#include "Api/Files/Models/FileData.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "PlayerZero.h"
#include "PlayerZeroFunctionLibrary.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterFindByIdResponse.h"
#include "Api/Files/GlbLoader.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

UPlayerZeroLoaderComponent::UPlayerZeroLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	AppId = PlayerZeroSettings->ApplicationId;
	FileApi = MakeShared<FFileApi>();
	//FileApi->OnAssetFileRequestComplete.BindUObject(this, &UPlayerZeroLoaderComponent::HandleAssetLoaded);
	FileApi->OnAssetFileRequestComplete.BindUObject(this, &UPlayerZeroLoaderComponent::HandleCharacterAssetLoaded);
	CharacterApi = MakeShared<FCharacterApi>();
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

void UPlayerZeroLoaderComponent::LoadCharacterFromUrl(FString Url)
{
	FileApi->LoadFileFromUrl(Url);
}

void UPlayerZeroLoaderComponent::HandleCharacterAssetLoaded(const FFileData& File, const TArray<unsigned char>& Data)
{
	if(Data.Num() <1)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to load character asset data"));
		return;
	}
	UglTFRuntimeAsset* GltfRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, GltfConfig);
	if(!GltfRuntimeAsset)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to load gltf asset"));
	}
	OnCharacterAssetLoaded.Broadcast(CharacterData, GltfRuntimeAsset);
}

void UPlayerZeroLoaderComponent::HandleCharacterFindResponse(FCharacterFindByIdResponse CharacterFindByIdResponse, bool bWasSuccessful)
{
	OnCharacterFound.Broadcast(CharacterData);
}

void UPlayerZeroLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

