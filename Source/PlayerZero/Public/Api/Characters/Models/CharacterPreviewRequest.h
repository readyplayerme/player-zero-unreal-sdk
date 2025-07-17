#pragma once

#include "CoreMinimal.h"
#include "CharacterPreviewRequest.generated.h"


USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterPreviewQueryParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "assets"))
	TMap<FString, FString> Assets;
};


USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterPreviewRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FString Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FCharacterPreviewQueryParams Params;
};
