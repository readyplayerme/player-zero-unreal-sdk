#include "Api/Assets/AssetApi.h"
#include "RpmNextGen.h"
#include "Settings/RpmDeveloperSettings.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Assets/Models/AssetTypeListRequest.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Cache/AssetCacheManager.h"

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
	const URpmDeveloperSettings* Settings = GetDefault<URpmDeveloperSettings>();
	ApiBaseUrl = Settings->GetApiBaseUrl();
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
		LoadAssetsFromCache(Request->BuildQueryMap(), OnComplete);
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
	FString QueryString = Request->BuildQueryString();
	const FString Url = FString::Printf(TEXT("%s/v1/phoenix-assets/types%s"), *ApiBaseUrl, *QueryString);
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Url;
	ApiRequest->Method = GET;
	SendRequestWithAuth<FAssetTypeListResponse>(ApiRequest, [OnComplete](TSharedPtr<FAssetTypeListResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
}

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
