#pragma once

#include "CoreMinimal.h"
#include "Api/Assets/Models/Asset.h"
#include "Api/Common/Models/ApiResponse.h"
#include "Api/Common/Models/Pagination.h"
#include "AssetListResponse.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAssetListResponse : public FApiResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "data"))
	TArray<FAsset> Data;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "pagination") )
	FPagination Pagination;
};
