
#pragma once

#include "CoreMinimal.h"

struct IGameSession
{
	virtual FString GetSessionId() const = 0;
	virtual void SetSessionId(const FString& InId) = 0;
};

struct IGame
{
	virtual FString GetGameId() const = 0;
	virtual void SetGameId(const FString& InId) = 0;
};

template<typename T>
struct IGameEvent
{
	virtual const T& GetProperties() const = 0;
	virtual FString GetToken() const = 0;
	virtual void SetToken(const FString& InToken) = 0;
};

template<typename TProps>
struct TGameEventWrapper
{
	FString Event;
	TProps Properties;
	FString Token;

	TSharedPtr<FJsonObject> ToWrappedJson() const
	{
		TSharedPtr<FJsonObject> Inner = MakeShared<FJsonObject>();
		Inner->SetStringField(TEXT("event"), Event);
		Inner->SetObjectField(TEXT("properties"), Properties.ToJson());
		Inner->SetStringField(TEXT("token"), Token);
		TSharedPtr<FJsonObject> Wrapper = MakeShared<FJsonObject>();
		Wrapper->SetObjectField(TEXT("data"), Inner);
		return Wrapper;
	}
};

template<typename T>
class TJsonSerializable
{
public:
	TSharedPtr<FJsonObject> ToJson() const
	{
		return static_cast<const T*>(this)->ToJson();
	}
};
