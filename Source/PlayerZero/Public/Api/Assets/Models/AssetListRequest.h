#pragma once

#include "CoreMinimal.h"
#include "Api/Common/Models/PaginationQueryParams.h"
#include "AssetListRequest.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAssetListQueryParams : public FPaginationQueryParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "applicationId"))
	FString ApplicationId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "type"))
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "excludeTypes"))
	TArray<FString> ExcludeTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "characterModelAssetId"))
	FString CharacterModelAssetId;
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAssetListRequest 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FAssetListQueryParams Params;

	FAssetListRequest()
	{
	}

	FAssetListRequest(const FAssetListQueryParams& InParams)
		: Params(InParams)
	{
	}

	TMap<FString, FString> BuildQueryMap() const;
};

inline TMap<FString, FString> FAssetListRequest::BuildQueryMap() const
{
	TMap<FString, FString> QueryMap;
	if (!Params.ApplicationId.IsEmpty())
	{
		QueryMap.Add(TEXT("applicationId"), Params.ApplicationId);
	}
	if (!Params.Type.IsEmpty())
	{
		QueryMap.Add(TEXT("type"), Params.Type);
	}
	if (!Params.ExcludeTypes.IsEmpty())
	{
		if (Params.ExcludeTypes.Num() > 0)
		{
			FString ExcludeTypesString;
			for (int32 i = 0; i < Params.ExcludeTypes.Num(); i++)
			{
				ExcludeTypesString += Params.ExcludeTypes[i];
				// Add '&excludeTypes=' only if it's not the last element
				if (i < Params.ExcludeTypes.Num() - 1)
				{
					ExcludeTypesString += TEXT("&excludeTypes=");
				}
			}
			QueryMap.Add(TEXT("excludeTypes"), ExcludeTypesString);
		}
	}
	if (!Params.CharacterModelAssetId.IsEmpty())
	{
		QueryMap.Add(TEXT("characterModelAssetId"), Params.CharacterModelAssetId);
	}
	if( Params.Limit > 0 )
	{
		QueryMap.Add(TEXT("limit"), FString::FromInt(Params.Limit));
	}
	if( Params.Page > 0 )
	{
		QueryMap.Add(TEXT("page"), FString::FromInt(Params.Page));
	}
	if( !Params.Order.IsEmpty() )
	{
		QueryMap.Add(TEXT("order"), Params.Order);
	}
	return QueryMap;
}
