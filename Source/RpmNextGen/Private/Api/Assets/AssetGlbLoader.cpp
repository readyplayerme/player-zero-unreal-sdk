﻿#include "Api/Assets/AssetGlbLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"
#include "Cache/AssetCacheManager.h"
#include "Interfaces/IHttpResponse.h"

FAssetGlbLoader::FAssetGlbLoader()
{
    Http = &FHttpModule::Get();
}

FAssetGlbLoader::~FAssetGlbLoader()
{
    CancelAllRequests();
}

void FAssetGlbLoader::LoadGlb(const FAsset& Asset, const FString& BaseModelId, bool bStoreInCache)
{
    FCachedAssetData StoredAsset;
    if (FAssetCacheManager::Get().GetCachedAsset(Asset.Id, StoredAsset))
    {
        TArray<uint8> GlbData;
        const FString StoredGlbPath = StoredAsset.GetGlbPathForBaseModelId(BaseModelId);
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
    SendRequest<TArray<uint8> >(ApiRequest, [ThisPtr, Asset, BaseModelId, bStoreInCache](TSharedPtr<TArray<uint8>> Response, bool bWasSuccessful, int32 StatusCode)
    {
        if (ThisPtr.IsValid())
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FAssetLoadingContext Context(Asset, BaseModelId, bStoreInCache);
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

// void FAssetGlbLoader::LoadGlb(const FAsset& Asset, const FString& BaseModelId, bool bStoreInCache)
// {
//     FCachedAssetData StoredAsset;
//     if (FAssetCacheManager::Get().GetCachedAsset(Asset.Id, StoredAsset))
//     {
//         TArray<uint8> GlbData;
//         const FString StoredGlbPath = StoredAsset.GetGlbPathForBaseModelId(BaseModelId);
//         if(FFileHelper::LoadFileToArray(GlbData, *StoredGlbPath))
//         {
//             OnGlbLoaded.ExecuteIfBound(Asset, GlbData);
//             return;
//         }
//     }
//     const TSharedRef<FAssetLoadingContext> Context = MakeShared<FAssetLoadingContext>(Asset, BaseModelId, bStoreInCache);
//     LoadGlb(Context);
// }

// void FAssetGlbLoader::LoadGlb(TSharedRef<FAssetLoadingContext> Context)
// {
//     TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//     Request->SetURL(Context->Asset.GlbUrl);
//     Request->SetVerb(TEXT("GET"));
//
//     TSharedPtr<FAssetGlbLoader> ThisPtr = SharedThis(this);
//     Request->OnProcessRequestComplete().BindLambda([ThisPtr, Context](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//     {
//         ThisPtr->GlbLoaded(Response, bWasSuccessful, Context);
//     });
//     Request->ProcessRequest();
// }
//
// void FAssetGlbLoader::GlbLoaded(TSharedPtr<IHttpResponse> Response, const bool bWasSuccessful, const TSharedRef<FAssetLoadingContext>& Context)
// {
//     if (bWasSuccessful && Response.IsValid())
//     {
//         Context->Data = Response->GetContent();
//         if (Context->bStoreInCache)
//         {
//             FAssetCacheManager::Get().StoreAndTrackGlb(*Context);
//         }
//         OnGlbLoaded.ExecuteIfBound(Context->Asset, Context->Data);
//         return;
//     }
//     UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load GLB from URL: %s"), *Context->Asset.GlbUrl);
//     OnGlbLoaded.ExecuteIfBound(Context->Asset, TArray<uint8>());
// }