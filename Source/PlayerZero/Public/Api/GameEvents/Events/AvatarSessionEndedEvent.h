#pragma once

#include "AvatarSessionEndedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FAvatarSessionEndedProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;
};

USTRUCT()
struct PLAYERZERO_API FAvatarSessionEnded
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString Event;

	UPROPERTY(meta = (JsonProperty = "properties"))
	FAvatarSessionEndedProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};