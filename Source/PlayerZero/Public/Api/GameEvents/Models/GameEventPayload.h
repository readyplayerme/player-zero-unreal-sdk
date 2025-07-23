#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameEventPayload.generated.h"

USTRUCT()
struct FGameEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Token;
};
