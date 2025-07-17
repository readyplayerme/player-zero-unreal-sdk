#pragma once

#include "CoreMinimal.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Common/Models/ApiResponse.h"
#include "Api/Common/Models/Pagination.h"
#include "AssetListResponse.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAssetListResponse : public FApiResponse
{
	GENERATED_BODY()

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Assets/Models/AssetListResponse.h
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "data"))
	TArray<FAsset> Data;
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "data"))
	TArray<FRpmAsset> Data;
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Assets/Models/AssetListResponse.h

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "pagination") )
	FPagination Pagination;
};
