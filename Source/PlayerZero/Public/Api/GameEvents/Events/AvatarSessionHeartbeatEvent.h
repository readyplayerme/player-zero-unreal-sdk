#pragma once

#include "AvatarSessionHeartbeatEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FAvatarSessionHeartbeatProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;

	UPROPERTY( meta = (JsonProperty = "last_avatar_activity_at"))
	int64 LastAvatarActivityAt;
};

USTRUCT()
struct PLAYERZERO_API FAvatarSessionHeartbeat
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString EventName;

	UPROPERTY(meta = (JsonProperty = "properties"))
	FAvatarSessionHeartbeatProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};