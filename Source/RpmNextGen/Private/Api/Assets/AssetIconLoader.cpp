#include "Api/Assets/AssetIconLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"
#include "Cache/AssetCacheManager.h"
#include "Interfaces/IHttpResponse.h"

FAssetIconLoader::FAssetIconLoader()
{
	Http = &FHttpModule::Get();
}

FAssetIconLoader::~FAssetIconLoader()
{
	CancelAllRequests();
}

void FAssetIconLoader::LoadIcon(const FAsset& Asset, bool bStoreInCache)
{
	FCachedAssetData StoredAsset;
	if (FAssetCacheManager::Get().GetCachedAsset(Asset.Id, StoredAsset))
	{
		TArray<uint8> IconData;
		if(FFileHelper::LoadFileToArray(IconData, * FFileUtility::GetFullPersistentPath(StoredAsset.RelativeIconFilePath)))
		{
			OnIconLoaded.ExecuteIfBound(Asset, IconData);
			return;
		}
	}
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Asset.IconUrl;
	ApiRequest->Method = GET;
	TWeakPtr<FAssetIconLoader> ThisPtr = SharedThis(this);
	SendRequest<TArray<uint8>>(ApiRequest, [ThisPtr, Asset, bStoreInCache](TSharedPtr<TArray<uint8>> Response, bool bWasSuccessful, int32 StatusCode)
	{
		if(ThisPtr.IsValid())
		{
			if (bWasSuccessful && Response.IsValid())
			{
				FAssetLoadingContext Context(Asset, "", bStoreInCache);
				Context.Data = *Response.Get();
				if (bStoreInCache)
				{
					FAssetCacheManager::Get().StoreAndTrackIcon(Context);
				}
				ThisPtr.Pin()->OnIconLoaded.ExecuteIfBound(Asset, Context.Data);
				return;
			}
			UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load icon from URL: %s"), *Asset.IconUrl);
			ThisPtr.Pin()->OnIconLoaded.ExecuteIfBound(Asset, TArray<uint8>());
		}
	});
}
