#pragma once

#include "AvatarSessionStartedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FAvatarSessionStartedProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString AvatarId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString AvatarType;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;
	
	UPROPERTY(meta = (JsonProperty = "game_session_id"))
	FString GameSessionId;
	
	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;
	
	UPROPERTY(meta = (JsonProperty = "sdk_version"))
	FString SdkVersion;
	
	UPROPERTY(meta = (JsonProperty = "sdk_platform"))
	FString SdkPlatform;
	
	// TODO add device context;
};

USTRUCT()
struct PLAYERZERO_API FAvatarSessionStartedEnded
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString Event = "avatar_session_started";

	UPROPERTY(meta = (JsonProperty = "properties"))
	FAvatarSessionStartedProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};