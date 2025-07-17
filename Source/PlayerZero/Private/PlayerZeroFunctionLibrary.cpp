// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroFunctionLibrary.h"
#include "PlayerZero.h"
#include "Api/Assets/AssetApi.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

void UPlayerZeroFunctionLibrary::FetchFirstAssetId(UObject* WorldContextObject, const FString& AssetType, FOnAssetIdFetched OnAssetIdFetched)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogPlayerZero, Error, TEXT("WorldContextObject is null"));
        OnAssetIdFetched.ExecuteIfBound(FString());
        return;
    }

    TSharedPtr<FAssetApi> AssetApi = MakeShared<FAssetApi>();
    TSharedPtr<FOnAssetIdFetched> SharedDelegate = MakeShared<FOnAssetIdFetched>(OnAssetIdFetched);

    const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
    FAssetListQueryParams QueryParams;
    QueryParams.Type = AssetType;
    QueryParams.ApplicationId = PlayerZeroSettings->ApplicationId;
    QueryParams.Limit = 1;
    FAssetListRequest AssetListRequest = FAssetListRequest(QueryParams);

    TWeakObjectPtr<UObject> WeakContextObject(WorldContextObject);

    AssetApi->OnListAssetsResponse.BindLambda([WeakContextObject, SharedDelegate, AssetApi, AssetType](const FAssetListResponse& Response, bool bWasSuccessful)
    {
        if (!WeakContextObject.IsValid())
        {
            UE_LOG(LogPlayerZero, Error, TEXT("WorldContextObject is no longer valid."));
            SharedDelegate->ExecuteIfBound(FString());
            return;
        }

        if (bWasSuccessful && Response.Data.Num() > 0)
        {
            FString FirstAssetId = Response.Data[0].Id;
            UE_LOG(LogPlayerZero, Warning, TEXT("FirstAssetId fetched: %s"), *FirstAssetId);
            SharedDelegate->ExecuteIfBound(FirstAssetId);
            return;
        }

        UE_LOG(LogPlayerZero, Error, TEXT("Failed to fetch FirstAssetId"));
        SharedDelegate->ExecuteIfBound(FString());
    });
    
    AssetApi->ListAssetsAsync(AssetListRequest);
}