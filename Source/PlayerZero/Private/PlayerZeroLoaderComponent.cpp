// Copyright Ready Player Me


#include "PlayerZeroLoaderComponent.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterFindByIdResponse.h"
#include "Api/Files/GlbLoader.h"
#include "Settings/RpmDeveloperSettings.h"
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmLoaderComponent.cpp

UPlayerZeroLoaderComponent::UPlayerZeroLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	AppId = PlayerZeroSettings->ApplicationId;
	FileApi = MakeShared<FFileApi>();
	CharacterApi = MakeShared<FCharacterApi>();
	CharacterData = FRpmCharacterData();
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmLoaderComponent.cpp
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


void URpmLoaderComponent::FindCharacterById(const FString CharacterId)
{
	TSharedPtr<FCharacterFindByIdRequest> CharacterFindByIdRequest = MakeShared<FCharacterFindByIdRequest>();
	CharacterFindByIdRequest->Id = CharacterId;
	CharacterApi->FindByIdAsync(CharacterFindByIdRequest, FOnCharacterFindResponse::CreateUObject(this, &URpmLoaderComponent::HandleCharacterFindResponse));
}

void UPlayerZeroLoaderComponent::LoadCharacterFromUrl(FString Url)
{
	FileApi->LoadFileFromUrl(Url);
}

void URpmLoaderComponent::HandleCharacterAssetLoaded(const TArray<uint8>& Data, const FString& FileName)
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmLoaderComponent.cpp
{
	if(Data.Num() == 0)
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

void URpmLoaderComponent::HandleCharacterFindResponse(TSharedPtr<FCharacterFindByIdResponse> CharacterFindByIdResponse, bool bWasSuccessful)
>>>>>>> origin/develop:Source/RpmNextGen/Private/RpmLoaderComponent.cpp
{
	OnCharacterFound.Broadcast(CharacterData);
}

void UPlayerZeroLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

