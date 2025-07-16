#include "Api/Assets/AssetApi.h"
#include "RpmNextGen.h"
#include "Settings/RpmDeveloperSettings.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Assets/Models/AssetTypeListRequest.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"

struct FCachedAssetData;
struct FAssetTypeListRequest;
const FString FAssetApi::CharacterStyleAssetType = TEXT("baseModel");

FAssetApi::FAssetApi() 
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
