#include "Api/Assets/AssetIconLoader.h"
#include "Api/Assets/AssetLoaderContext.h"
#include "HttpModule.h"
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetIconLoader.cpp
#include "Interfaces/IHttpResponse.h"
=======
#include "Cache/AssetCacheManager.h"
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetIconLoader.cpp

FAssetIconLoader::FAssetIconLoader()
{
	Http = &FHttpModule::Get();
}

FAssetIconLoader::~FAssetIconLoader()
{
	CancelAllRequests();
}

void FAssetIconLoader::LoadIcon(const FRpmAsset& Asset, bool bStoreInCache)
{
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Assets/AssetIconLoader.cpp
	//TODO cleanup
	// FCachedAssetData StoredAsset;
	// if (FAssetCacheManager::Get().GetCachedAsset(Asset.Id, StoredAsset))
	// {
	// 	TArray<uint8> IconData;
	// 	if(FFileHelper::LoadFileToArray(IconData, * FFileUtility::GetFullPersistentPath(StoredAsset.RelativeIconFilePath)))
	// 	{
	// 		OnIconLoaded.ExecuteIfBound(Asset, IconData);
	// 		return;
	// 	}
	// }
	const TSharedRef<FAssetLoadingContext> Context = MakeShared<FAssetLoadingContext>(Asset, "", bStoreInCache);
	LoadIcon(Context);
}

void FAssetIconLoader::LoadIcon(TSharedRef<FAssetLoadingContext> Context)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(Context->Asset.IconUrl);
	Request->SetVerb(TEXT("GET"));

	TSharedPtr<FAssetIconLoader> ThisPtr = SharedThis(this);
	Request->OnProcessRequestComplete().BindLambda([ThisPtr, Context](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		ThisPtr->IconLoaded(Response, bWasSuccessful, Context);
	});
	Request->ProcessRequest();
}

void FAssetIconLoader::IconLoaded(TSharedPtr<IHttpResponse> Response, bool bWasSuccessful,	const TSharedRef<FAssetLoadingContext>& Context)
{
	if (bWasSuccessful && Response.IsValid())
	{
		Context->Data = Response->GetContent();
		//TODO cleanup
		// if (Context->bStoreInCache)
		// {
		// 	FAssetCacheManager::Get().StoreAndTrackIcon(*Context);
		// }
		OnIconLoaded.ExecuteIfBound(Context->Asset, Context->Data);
		return;
	}
	UE_LOG(LogPlayerZero, Error, TEXT("Failed to load icon from URL: %s"), *Context->Asset.IconUrl);
	OnIconLoaded.ExecuteIfBound(Context->Asset, TArray<uint8>());
=======
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
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Assets/AssetIconLoader.cpp
}
