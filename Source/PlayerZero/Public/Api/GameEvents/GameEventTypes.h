
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
