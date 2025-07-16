#include "Api/Assets/AssetGlbLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"

FAssetGlbLoader::FAssetGlbLoader()
{
    Http = &FHttpModule::Get();
}

FAssetGlbLoader::~FAssetGlbLoader()
{
    CancelAllRequests();
}

void FAssetGlbLoader::LoadGlb(const FRpmAsset& Asset, const FString& CharacterStyleId)
{

    TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
    ApiRequest->Url = Asset.GlbUrl;
    ApiRequest->Method = GET;

    TWeakPtr<FAssetGlbLoader> ThisPtr = SharedThis(this);
    SendRequest<TArray<uint8> >(ApiRequest, [ThisPtr, Asset, CharacterStyleId](TSharedPtr<TArray<uint8>> Response, bool bWasSuccessful, int32 StatusCode)
    {
        if (ThisPtr.IsValid())
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FAssetLoadingContext Context(Asset, CharacterStyleId);
                Context.Data = *Response.Get();

                ThisPtr.Pin()->OnGlbLoaded.ExecuteIfBound(Asset, Context.Data);
                return;
            }
            UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load GLB from URL: %s"), *Asset.GlbUrl);
            ThisPtr.Pin()->OnGlbLoaded.ExecuteIfBound(Asset, TArray<uint8>());
        }
    });
}