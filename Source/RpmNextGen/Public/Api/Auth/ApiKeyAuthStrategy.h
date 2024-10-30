﻿#pragma once

#include "CoreMinimal.h"
#include "Api/Auth/IAuthenticationStrategy.h"
#include "Api/Common/WebApi.h"

class RPMNEXTGEN_API FApiKeyAuthStrategy : public IAuthenticationStrategy
{
public:
	FApiKeyAuthStrategy();
	FApiKeyAuthStrategy(const FString& InApiKey);
	
	virtual void AddAuthToRequest(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, bool)> OnAuthComplete) override
	{
		if(ApiKey.IsEmpty())
		{
			UE_LOG(LogReadyPlayerMe, Error, TEXT("Api Key not set!") );
			OnAuthComplete(ApiRequest, false);
			return;
		}
		ApiRequest->Headers.Add(TEXT("X-API-Key"), ApiKey);

		OnAuthComplete(ApiRequest, true);
	}

	virtual void TryRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, const FRefreshTokenResponse&, bool)> OnTokenRefreshed) override
	{
		if(ApiKey.IsEmpty())
		{
			UE_LOG(LogReadyPlayerMe, Error, TEXT("No API key provided for refresh") );
		}
		OnTokenRefreshed(ApiRequest, FRefreshTokenResponse(), !ApiKey.IsEmpty());
	}
	
protected:
	FString ApiKey;
};
