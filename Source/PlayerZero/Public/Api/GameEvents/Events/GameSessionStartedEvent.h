#pragma once

#include "GameSessionStartedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FGameSessionStartedEventProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;

	UPROPERTY(meta = (JsonProperty = "avatar_id"))
	FString AvatarId;
	
	UPROPERTY(meta = (JsonProperty = "application_id"))
	FString ApplicationId;
	
	UPROPERTY(meta = (JsonProperty = "product_name"))
	FString ProductName;

	UPROPERTY(meta = (JsonProperty = "game_entry_point"))
	FString GameEntryPoint;

	UPROPERTY(meta = (JsonProperty = "live_ops_id"))
	FString LiveOpsId;
};

USTRUCT()
struct PLAYERZERO_API FGameSessionStartedEvent
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString Event = "game_match_ended";

	UPROPERTY(meta = (JsonProperty = "properties"))
	FGameSessionStartedEventProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};
