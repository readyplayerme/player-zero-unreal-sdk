#pragma once
#include "Api/Common/WebApiWithAuth.h"
#include "Models/AssetTypeListResponse.h"

class URpmDeveloperSettings;
struct FApiRequest;
struct FAssetTypeListRequest;
struct FAssetListRequest;
struct FAssetListResponse;

DECLARE_DELEGATE_TwoParams(FOnListAssetsResponse, TSharedPtr<FAssetListResponse>, bool);
DECLARE_DELEGATE_TwoParams(FOnListAssetTypesResponse, TSharedPtr<FAssetTypeListResponse>, bool);

class RPMNEXTGEN_API FAssetApi :  public FWebApiWithAuth
{
public:
	static const FString CharacterStyleAssetType;
	
	FAssetApi();
	virtual ~FAssetApi() override;
	void Initialize();
	virtual void SetAuthenticationStrategy(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy) override;
	void ListAssetsAsync(TSharedPtr<FAssetListRequest> Request, FOnListAssetsResponse OnComplete);
	void ListAssetTypesAsync(TSharedPtr<FAssetTypeListRequest> Request, FOnListAssetTypesResponse OnComplete);

private:
	FString ApiBaseUrl;

	TArray<FString> ExtractQueryValues(const FString& QueryString, const FString& Key);
};
