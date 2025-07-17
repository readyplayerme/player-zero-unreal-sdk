#pragma once
#include "CharacterBlueprint.h"
#include "Api/Common/Models/Pagination.h"
#include "BlueprintListResponse.generated.h"


USTRUCT(BlueprintType)
struct RPMNEXTGEN_API FBlueprintListResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "data"))
	TArray<FCharacterBlueprint> Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "pagination"))
	FPagination Pagination;
	
};
