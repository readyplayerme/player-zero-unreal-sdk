#pragma once

#include "CoreMinimal.h"
#include "PlayerZeroCharacter.h"
#include "Api/Common/Models/ApiResponse.h"
#include "CharacterCreateResponse.generated.h"

USTRUCT()
struct PLAYERZERO_API FCharacterCreateResponse : public FApiResponse
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "data"))
	FPlayerZeroCharacter Data;

	FCharacterCreateResponse()
	{		
	}

	FCharacterCreateResponse(FPlayerZeroCharacter Data)
	{
		this->Data = Data;
	}

	bool IsValid() const
	{
		return !Data.Id.IsEmpty() && !Data.GlbUrl.IsEmpty();
	}
};
