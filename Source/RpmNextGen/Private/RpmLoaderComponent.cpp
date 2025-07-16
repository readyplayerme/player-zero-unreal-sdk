// Copyright Ready Player Me


#include "RpmLoaderComponent.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterFindByIdResponse.h"
#include "Api/Files/GlbLoader.h"
#include "Settings/RpmDeveloperSettings.h"

URpmLoaderComponent::URpmLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	AppId = RpmSettings->ApplicationId;
	FileApi = MakeShared<FFileApi>();
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

void URpmLoaderComponent::HandleCharacterFindResponse(TSharedPtr<FCharacterFindByIdResponse> CharacterFindByIdResponse, bool bWasSuccessful)
{
	OnCharacterFound.Broadcast(CharacterData);
}

void URpmLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

