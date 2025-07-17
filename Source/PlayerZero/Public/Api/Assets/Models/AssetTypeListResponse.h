#pragma once

#include "CoreMinimal.h"
#include "Api/Common/Models/ApiResponse.h"
#include "AssetTypeListResponse.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAssetTypeListResponse : public FApiResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "data"))
	TArray<FString> Data;
};
