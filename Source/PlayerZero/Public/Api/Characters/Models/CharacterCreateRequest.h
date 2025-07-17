#pragma once

#include "CoreMinimal.h"
#include "Api/Common/Models/ApiResponse.h"
#include "CharacterCreateRequest.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterCreateRequestBody
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "applicationId"))
	FString ApplicationId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "assets"))
	TMap<FString, FString> Assets;
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FCharacterCreateRequestBody Data;
};