#pragma once

#include "GameSessionEndedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FGameSessionEndedEventProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;

	UPROPERTY(meta = (JsonProperty = "matches_played"))
	int32 MatchesPlayed;

	UPROPERTY(meta = (JsonProperty = "matches_won"))
	int32 MatchesWon;

	UPROPERTY(meta = (JsonProperty = "score"))
	int32 score;
	
	UPROPERTY(meta = (JsonProperty = "currency_obtained"))
	TMap<FString, FString> CurrencyObtained = TMap<FString, FString>();
};

USTRUCT()
struct PLAYERZERO_API FGameSessionEndedEvent
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString Event = "game_match_ended";

	UPROPERTY(meta = (JsonProperty = "properties"))
	FGameSessionEndedEventProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};
