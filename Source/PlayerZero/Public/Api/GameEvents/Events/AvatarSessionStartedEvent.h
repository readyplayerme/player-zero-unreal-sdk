#pragma once

#include "AvatarSessionStartedEvent.generated.h"

USTRUCT()
struct PLAYERZERO_API FAvatarSessionStartedProperties
{
	GENERATED_BODY()

	UPROPERTY()
	FString AvatarId;

	UPROPERTY()
	FString AvatarType = "fullbody";

	UPROPERTY()
	FString GameId;
	
	UPROPERTY()
	FString GameSessionId;
	
	UPROPERTY()
	FString SessionId;
	
	UPROPERTY()
	FString SdkVersion;
	
	UPROPERTY()
	FString SdkPlatform;
	
	// TODO add device context;

	TSharedPtr<FJsonObject> ToJson() const
	{
		TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
		Json->SetStringField(TEXT("avatar_id"), AvatarId);
		Json->SetStringField(TEXT("avatar_type"), AvatarType);
		Json->SetStringField(TEXT("game_id"), GameId);
		Json->SetStringField(TEXT("game_session_id"), GameSessionId);
		Json->SetStringField(TEXT("avatar_session_id"), SessionId);
		Json->SetStringField(TEXT("sdk_version"), SdkVersion);
		Json->SetStringField(TEXT("sdk_platform"), SdkPlatform);
		return Json;
	}
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