#pragma once
#include "Api/Assets/Models/AssetListResponse.h"
#include "Api/Assets/Models/AssetListRequest.h"

class FAssetGlbLoader;
class FAssetIconLoader;
class FTaskManager;
struct FCachedAssetData;
class FAssetSaver;
struct FAssetTypeListResponse;
class FAssetApi;
struct FRpmAsset;
class IHttpResponse;
class IHttpRequest;
class FHttpModule;

DECLARE_DELEGATE_OneParam(FOnCacheDataLoaded, bool);
DECLARE_DELEGATE_OneParam(FOnLocalCacheGenerated, bool);
DECLARE_DELEGATE_OneParam(FOnDownloadRemoteCache, bool);

class RPMNEXTGENEDITOR_API FCacheGenerator : public TSharedFromThis<FCacheGenerator>
{
public:
	FOnDownloadRemoteCache OnDownloadRemoteCacheDelegate;
	FOnCacheDataLoaded OnCacheDataLoaded;
	FOnLocalCacheGenerated OnLocalCacheGenerated;
	
	FCacheGenerator();
	virtual ~FCacheGenerator();
	
	void DownloadRemoteCacheFromUrl(const FString& Url);
	void GenerateLocalCache(int InItemsPerCategory);
	void LoadAndStoreAssets();
	void LoadAndStoreAssetGlb(const FString& CharacterStyleId, const FRpmAsset* Asset);
	void LoadAndStoreAssetIcon(const FString& CharacterStyleId, const FRpmAsset* Asset);
	void Reset();

protected:
	TSharedPtr<FAssetApi> AssetApi;
	TArray<FString> AssetTypes;
	TMap<FString, TArray<FRpmAsset>> AssetMapByCharacterStyleId;
	TArray<TSharedPtr<FAssetListRequest>> AssetListRequests;
	int32 CurrentCharacterStyleIndex;
	
	UFUNCTION()
	void OnAssetGlbSaved(const FRpmAsset& Asset, const TArray<uint8>& Data);
	UFUNCTION()
	void OnAssetIconSaved(const FRpmAsset& Asset, const TArray<uint8>& Data);

	void AddFolderToNonAssetDirectory() const;
	void FetchStyleAssets();
	void FetchAssetTypes();
	void FetchNextRefittedAsset();
	
	virtual void OnDownloadRemoteCacheComplete(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bWasSuccessful);
	
private:
	static const FString ZipFileName;
	TArray<TSharedPtr<FAssetGlbLoader>> ActiveGlbLoaders;
	TArray<TSharedPtr<FAssetIconLoader>> ActiveIconLoaders;
	FHttpModule* Http;
	
	int MaxItemsPerCategory;
	int RefittedAssetRequestsCompleted = 0;
	int RequiredAssetDownloadRequest = 0;
	int NumberOfAssetsSaved = 0;
	
	void StartFetchingRefittedAssets();
	void OnListCharacterStylesComplete(TSharedPtr<FAssetListResponse> AssetListResponse, bool bWasSuccessful);
	void OnListAssetsComplete(TSharedPtr<FAssetListResponse> AssetListResponse, bool bWasSuccessful);
	void OnListAssetTypesComplete(TSharedPtr<FAssetTypeListResponse> AssetTypeListResponse, bool bWasSuccessful);
};
