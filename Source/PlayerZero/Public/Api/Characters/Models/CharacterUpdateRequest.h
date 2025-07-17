#pragma once

#include "CoreMinimal.h"
#include "CharacterUpdateRequest.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterUpdateRequestBody
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "assets"))
	TMap<FString, FString> Assets;
};


USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterUpdateRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "applicationId"))
	FString Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FCharacterUpdateRequestBody Payload;
};
