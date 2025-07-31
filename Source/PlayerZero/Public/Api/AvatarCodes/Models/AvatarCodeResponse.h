#pragma once

#include "CoreMinimal.h"
#include "Api/Common/Models/ApiResponse.h"
#include "AvatarCodeResponse.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAvatarCodeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PlayerZero|AvatarCode")
	FString AvatarId;
	
	UPROPERTY(BlueprintReadOnly, Category = "PlayerZero|AvatarCode")
	FString Code;
	
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAvatarCodeResponse : public FApiResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PlayerZero|AvatarCode")
	FAvatarCodeData Data;
};
