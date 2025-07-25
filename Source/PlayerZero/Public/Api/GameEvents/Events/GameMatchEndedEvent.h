#pragma once

#include "GameMatchEndedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FGameMatchEndedProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;

	UPROPERTY(meta = (JsonProperty = "score"))
	int32 Score;
	
	UPROPERTY(meta = (JsonProperty = "outcome"))
	int32 Outcome;
	
	UPROPERTY(meta = (JsonProperty = "currency_obtained"))
	TMap<FString, FString> CurrencyObtained;
};

USTRUCT()
struct PLAYERZERO_API FGameMatchEnded
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString Event = "game_match_ended";

	UPROPERTY(meta = (JsonProperty = "properties"))
	FGameMatchEndedProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};