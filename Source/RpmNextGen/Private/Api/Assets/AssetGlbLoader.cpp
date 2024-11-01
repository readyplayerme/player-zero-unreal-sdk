#include "Api/Assets/AssetGlbLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"
#include "Cache/AssetCacheManager.h"

FAssetGlbLoader::FAssetGlbLoader()
{
    Http = &FHttpModule::Get();
}

FAssetGlbLoader::~FAssetGlbLoader()
{
    CancelAllRequests();
}

void FAssetGlbLoader::LoadGlb(const FRpmAsset& Asset, const FString& CharacterStyleId, bool bStoreInCache)
{
    FCachedAssetData StoredAsset;
    if (FAssetCacheManager::Get().GetCachedAsset(Asset.Id, StoredAsset))
    {
        TArray<uint8> GlbData;
        const FString StoredGlbPath = StoredAsset.GetGlbPathForCharacterStyleId(CharacterStyleId);
        if(FFileHelper::LoadFileToArray(GlbData, *StoredGlbPath))
        {
            OnGlbLoaded.ExecuteIfBound(Asset, GlbData);
            return;
        }
    }

    TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
    ApiRequest->Url = Asset.GlbUrl;
    ApiRequest->Method = GET;

    TWeakPtr<FAssetGlbLoader> ThisPtr = SharedThis(this);
    SendRequest<TArray<uint8> >(ApiRequest, [ThisPtr, Asset, CharacterStyleId, bStoreInCache](TSharedPtr<TArray<uint8>> Response, bool bWasSuccessful, int32 StatusCode)
    {
        if (ThisPtr.IsValid())
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FAssetLoadingContext Context(Asset, CharacterStyleId, bStoreInCache);
                Context.Data = *Response.Get();
                if (bStoreInCache)
                {
                    FAssetCacheManager::Get().StoreAndTrackGlb(Context);
                }
                ThisPtr.Pin()->OnGlbLoaded.ExecuteIfBound(Asset, Context.Data);
                return;
            }
            UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load GLB from URL: %s"), *Asset.GlbUrl);
            ThisPtr.Pin()->OnGlbLoaded.ExecuteIfBound(Asset, TArray<uint8>());
        }
    });
}