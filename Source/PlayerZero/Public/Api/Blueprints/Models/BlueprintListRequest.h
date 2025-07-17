#pragma once

#include "Api/Common/Models/PaginationQueryParams.h"
#include "BlueprintListRequest.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FBlueprintListRequest : public FPaginationQueryParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "applicationId"))
	FString ApplicationId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "archived"))
	bool Archived;

	FBlueprintListRequest()
	{
	}
	
	FBlueprintListRequest(const FString& InApplicationId, bool bInArchived = false)
		: ApplicationId(InApplicationId), Archived(bInArchived)
	{
	}
};

