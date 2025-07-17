#include "Api/Assets/AssetIconLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"

FAssetIconLoader::FAssetIconLoader()
{
	Http = &FHttpModule::Get();
}

FAssetIconLoader::~FAssetIconLoader()
{
	CancelAllRequests();
}

void FAssetIconLoader::LoadIcon(const FRpmAsset& Asset)
{
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Asset.IconUrl;
	ApiRequest->Method = GET;
	TWeakPtr<FAssetIconLoader> ThisPtr = SharedThis(this);
	SendRequest<TArray<uint8>>(ApiRequest, [ThisPtr, Asset](TSharedPtr<TArray<uint8>> Response, bool bWasSuccessful, int32 StatusCode)
	{
		if(ThisPtr.IsValid())
		{
			if (bWasSuccessful && Response.IsValid())
			{
				FAssetLoadingContext Context(Asset, "");
				Context.Data = *Response.Get();
				ThisPtr.Pin()->OnIconLoaded.ExecuteIfBound(Asset, Context.Data);
				return;
			}
			UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load icon from URL: %s"), *Asset.IconUrl);
			ThisPtr.Pin()->OnIconLoaded.ExecuteIfBound(Asset, TArray<uint8>());
		}
	});
}
