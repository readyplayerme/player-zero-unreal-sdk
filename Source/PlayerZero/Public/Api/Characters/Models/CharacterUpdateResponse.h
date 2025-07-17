#pragma once

#include "CoreMinimal.h"
#include "PlayerZeroCharacter.h"
#include "Api/Common/Models/ApiResponse.h"
#include "CharacterUpdateResponse.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterUpdateResponse : public FApiResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "data"))
	FPlayerZeroCharacter Data;
};
