#pragma once

#include "GameMatchStartedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FGameMatchStartedEventProperties
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "session_id"))
	FString SessionId;

	UPROPERTY(meta = (JsonProperty = "game_id"))
	FString GameId;

	UPROPERTY(meta = (JsonProperty = "avatar_id"))
	FString AvatarId;

	UPROPERTY(meta = (JsonProperty = "product_name"))
	FString ProductName;

	UPROPERTY(meta = (JsonProperty = "game_session_id"))
	FString GameSessionId;

	UPROPERTY(meta = (JsonProperty = "start_context"))
	FString StartContext;

	UPROPERTY(meta = (JsonProperty = "live_ops_id"))
	FString LiveOpsId;
	
	UPROPERTY(meta = (JsonProperty = "tier"))
	FString Tier;
	
	UPROPERTY(meta = (JsonProperty = "round"))
	int32 Round;

	UPROPERTY(meta = (JsonProperty = "map_id"))
	FString MapId;

	UPROPERTY(meta = (JsonProperty = "game_mode"))
	FString GameMode;

	UPROPERTY(meta = (JsonProperty = "class"))
	FString Class;

	UPROPERTY(meta = (JsonProperty = "team"))
	FString Team;
	
	UPROPERTY(meta = (JsonProperty = "loadout"))
	TMap<FString, FString> Loadout;

	UPROPERTY(meta = (JsonProperty = "lobby_id"))
	FString LobbyId;
};

USTRUCT()
struct PLAYERZERO_API FGameMatchStartedEvent
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "event"))
	FString Event = "game_match_started";

	UPROPERTY(meta = (JsonProperty = "properties"))
	FGameMatchStartedEventProperties Properties;

	UPROPERTY(meta = (JsonProperty = "token"))
	FString Token;
};