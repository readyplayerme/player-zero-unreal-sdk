#include "Api/Assets/AssetApi.h"
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetApi.cpp

#include "PlayerZero.h"
#include "Settings/PlayerZeroDeveloperSettings.h"
=======
#include "RpmNextGen.h"
#include "Settings/RpmDeveloperSettings.h"
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetApi.cpp
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Assets/Models/AssetTypeListRequest.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetApi.cpp
#include "Interfaces/IHttpResponse.h"
=======
#include "Cache/AssetCacheManager.h"
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetApi.cpp

struct FCachedAssetData;
struct FAssetTypeListRequest;
const FString FAssetApi::CharacterStyleAssetType = TEXT("baseModel");

FAssetApi::FAssetApi() : ApiRequestStrategy(EApiRequestStrategy::FallbackToCache)
{
	Initialize();
}

FAssetApi::FAssetApi(EApiRequestStrategy InApiRequestStrategy) : ApiRequestStrategy(InApiRequestStrategy)
{
	Initialize();
}

FAssetApi::~FAssetApi()
{
	CancelAllRequests();
}

void FAssetApi::Initialize()
{
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetApi.cpp
	if (bIsInitialized)	return;
	
	const UPlayerZeroDeveloperSettings* Settings = GetDefault<UPlayerZeroDeveloperSettings>();
	OnRequestComplete.BindRaw(this, &FAssetApi::HandleAssetResponse);
	if (Settings->ApplicationId.IsEmpty())
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Application ID is empty. Please set the Application ID in the Ready Player Me Developer Settings"));
	}

	if (!Settings->ApiKey.IsEmpty() || Settings->ApiProxyUrl.IsEmpty())
	{
		SetAuthenticationStrategy(MakeShared<FApiKeyAuthStrategy>());
	}
	bIsInitialized = true;
=======
	const URpmDeveloperSettings* Settings = GetDefault<URpmDeveloperSettings>();
	ApiBaseUrl = Settings->GetApiBaseUrl();
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetApi.cpp
}

void FAssetApi::SetAuthenticationStrategy(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy)
{
	FWebApiWithAuth::SetAuthenticationStrategy(InAuthenticationStrategy);
	Initialize();
}

void FAssetApi::ListAssetsAsync(TSharedPtr<FAssetListRequest> Request, FOnListAssetsResponse OnComplete)
{
	if(ApiRequestStrategy == EApiRequestStrategy::CacheOnly)
	{
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetApi.cpp
		LoadAssetsFromCache(Request.BuildQueryMap());
		return;
	}
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	ApiBaseUrl = PlayerZeroSettings->GetApiBaseUrl();
	if(PlayerZeroSettings->ApplicationId.IsEmpty())
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Application ID is empty"));
		OnListAssetsResponse.ExecuteIfBound(FAssetListResponse(), false);
=======
		LoadAssetsFromCache(Request->BuildQueryMap(), OnComplete);
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetApi.cpp
		return;
	}
	const FString Url = FString::Printf(TEXT("%s/v1/phoenix-assets"), *ApiBaseUrl);
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Url;
	ApiRequest->Method = GET;
	ApiRequest->QueryParams = Request->BuildQueryMap();

	SendRequestWithAuth<FAssetListResponse>(ApiRequest, [OnComplete](TSharedPtr<FAssetListResponse> Response, bool bWasSuccessful, int32 StatusCode)
	 {
		 OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	 });
}

void FAssetApi::ListAssetTypesAsync(TSharedPtr<FAssetTypeListRequest> Request, FOnListAssetTypesResponse OnComplete)
{
	if(ApiRequestStrategy == EApiRequestStrategy::CacheOnly)
	{
		LoadAssetTypesFromCache(OnComplete);
		return;
	}
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetApi.cpp
	UPlayerZeroDeveloperSettings* Settings = GetMutableDefault<UPlayerZeroDeveloperSettings>();
	ApiBaseUrl = Settings->GetApiBaseUrl();
	if(Settings->ApplicationId.IsEmpty())
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Application ID is empty"));
		OnListAssetsResponse.ExecuteIfBound(FAssetListResponse(), false);
		return;
	}
	FString QueryString = Request.BuildQueryString();
=======
	FString QueryString = Request->BuildQueryString();
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetApi.cpp
	const FString Url = FString::Printf(TEXT("%s/v1/phoenix-assets/types%s"), *ApiBaseUrl, *QueryString);
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Url;
	ApiRequest->Method = GET;
	SendRequestWithAuth<FAssetTypeListResponse>(ApiRequest, [OnComplete](TSharedPtr<FAssetTypeListResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
}

<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetApi.cpp
void FAssetApi::HandleAssetResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful)
{
	const bool bIsTypeRequest = ApiRequest->Url.Contains(TEXT("/types"));
	if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAssetTypeListResponse AssetTypeListResponse;
		
		if (bIsTypeRequest && FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &AssetTypeListResponse, 0, 0))
		{
			OnListAssetTypeResponse.ExecuteIfBound(AssetTypeListResponse, true);
			return;
		}
		FAssetListResponse AssetListResponse;
		if (!bIsTypeRequest && FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &AssetListResponse, 0, 0))
		{
			OnListAssetsResponse.ExecuteIfBound(AssetListResponse, true);
			return;
		}
	}
	
	if(ApiRequestStrategy == EApiRequestStrategy::ApiOnly)
	{
		if(bIsTypeRequest)
		{
			UE_LOG(LogPlayerZero, Error, TEXT("AssetApi:ListAssetTypesAsync request failed: %s"), *Response->GetContentAsString());
			OnListAssetTypeResponse.ExecuteIfBound(FAssetTypeListResponse(), false);
			return;
		}
		UE_LOG(LogPlayerZero, Error, TEXT("AssetApi:ListAssetsAsync request failed: %s"), *Response->GetContentAsString());
		OnListAssetsResponse.ExecuteIfBound(FAssetListResponse(), false);
		return;
	}
	if(bIsTypeRequest)
	{
		UE_LOG(LogPlayerZero, Warning, TEXT("FAssetApi::ListAssetTypesAsync request failed"));
	}
=======
void FAssetApi::LoadAssetsFromCache(TMap<FString, FString> QueryParams, FOnListAssetsResponse OnComplete)
{
	if(QueryParams.Num() < 1)
	{
		OnComplete.ExecuteIfBound(MakeShared<FAssetListResponse>(), false);
		return;
	}
	const FString TypeKey = TEXT("type");
	const FString ExcludeTypeKey = TEXT("excludeType");
	FString Type = QueryParams.Contains(TypeKey) ? *QueryParams.Find(TypeKey) : FString();
	FString ExcludeType = QueryParams.Contains(ExcludeTypeKey) ? *QueryParams.Find(ExcludeTypeKey) : FString();
	TArray<FCachedAssetData> CachedAssets;
	
	if(ExcludeType.IsEmpty())
	{
		CachedAssets = FAssetCacheManager::Get().GetAssetsOfType(Type);
	}
	else
	{
		auto ExtractQueryString = TEXT("excludeType=") + ExcludeType;
		auto ExtractQueryArray = ExtractQueryValues(ExcludeType, ExcludeTypeKey);
		CachedAssets = FAssetCacheManager::Get().GetAssetsExcludingTypes(ExtractQueryArray);
	}

	if (CachedAssets.Num() > 0)
	{
		TSharedPtr<FAssetListResponse> AssetListResponse = MakeShared<FAssetListResponse>();
		for (const FCachedAssetData& CachedAsset : CachedAssets)
		{
			FRpmAsset Asset = CachedAsset.ToAsset();
			AssetListResponse->Data.Add(Asset);
		}
		OnComplete.ExecuteIfBound(AssetListResponse, true);
		return;
	}
	UE_LOG(LogReadyPlayerMe, Warning, TEXT("No assets found in the cache."));
	OnComplete.ExecuteIfBound(nullptr, false);
}

void FAssetApi::LoadAssetTypesFromCache(FOnListAssetTypesResponse OnComplete)
{
	TArray<FString> AssetTypes = FAssetCacheManager::Get().LoadAssetTypes();
	if (AssetTypes.Num() > 0)
	{
		TSharedPtr<FAssetTypeListResponse> AssetListResponse = MakeShared<FAssetTypeListResponse>();
		AssetListResponse->Data.Append(AssetTypes);
		OnComplete.ExecuteIfBound(AssetListResponse, true);
		return;
	}
	UE_LOG(LogReadyPlayerMe, Warning, TEXT("No assets found in the cache."));
	OnComplete.ExecuteIfBound(nullptr, false);
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetApi.cpp
}

TArray<FString> FAssetApi::ExtractQueryValues(const FString& QueryString, const FString& Key)
{
	TArray<FString> Values;
	TArray<FString> Pairs;
	
	// Split the query string by '&' to separate each key-value pair
	QueryString.ParseIntoArray(Pairs, TEXT("&"), true);

	// Iterate through all pairs and check if they match the key
	for (const FString& Pair : Pairs)
	{
		FString KeyPart, ValuePart;

		// Split the pair by '=' to get the key and value
		if (Pair.Split(TEXT("="), &KeyPart, &ValuePart))
		{
			// If the key matches the one we're looking for, add the value to the array
			if (KeyPart.Equals(Key, ESearchCase::IgnoreCase))
			{
				Values.Add(ValuePart);
			}
		}
	}

	return Values;
}
