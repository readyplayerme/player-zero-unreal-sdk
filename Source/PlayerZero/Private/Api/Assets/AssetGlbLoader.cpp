#include "Api/Assets/AssetGlbLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetGlbLoader.cpp
#include "Interfaces/IHttpResponse.h"
=======
#include "Cache/AssetCacheManager.h"
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetGlbLoader.cpp

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
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetGlbLoader.cpp
    //TODO cleanup
    //FCachedAssetData StoredAsset;
    // if (FAssetCacheManager::Get().GetCachedAsset(Asset.Id, StoredAsset))
    // {
    //     TArray<uint8> GlbData;
    //     const FString StoredGlbPath = StoredAsset.GetGlbPathForBaseModelId(BaseModelId);
    //     if(FFileHelper::LoadFileToArray(GlbData, *StoredGlbPath))
    //     {
    //         OnGlbLoaded.ExecuteIfBound(Asset, GlbData);
    //         return;
    //     }
    // }
    const TSharedRef<FAssetLoadingContext> Context = MakeShared<FAssetLoadingContext>(Asset, BaseModelId, bStoreInCache);
    LoadGlb(Context);
}
=======
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
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetGlbLoader.cpp

    TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
    ApiRequest->Url = Asset.GlbUrl;
    ApiRequest->Method = GET;

    TWeakPtr<FAssetGlbLoader> ThisPtr = SharedThis(this);
    SendRequest<TArray<uint8> >(ApiRequest, [ThisPtr, Asset, CharacterStyleId, bStoreInCache](TSharedPtr<TArray<uint8>> Response, bool bWasSuccessful, int32 StatusCode)
    {
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetGlbLoader.cpp
        ThisPtr->GlbLoaded(Response, bWasSuccessful, Context);
    });
    Request->ProcessRequest();
}

void FAssetGlbLoader::GlbLoaded(TSharedPtr<IHttpResponse> Response, const bool bWasSuccessful, const TSharedRef<FAssetLoadingContext>& Context)
{
    if (bWasSuccessful && Response.IsValid())
    {
        Context->Data = Response->GetContent();
        //TODO cleanup
        // if (Context->bStoreInCache)
        // {
        //     FAssetCacheManager::Get().StoreAndTrackGlb(*Context);
        // }
        OnGlbLoaded.ExecuteIfBound(Context->Asset, Context->Data);
        return;
    }
    UE_LOG(LogPlayerZero, Error, TEXT("Failed to load GLB from URL: %s"), *Context->Asset.GlbUrl);
    OnGlbLoaded.ExecuteIfBound(Context->Asset, TArray<uint8>());
=======
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
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetGlbLoader.cpp
}