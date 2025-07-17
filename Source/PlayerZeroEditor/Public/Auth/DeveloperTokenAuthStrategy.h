#pragma once

#include "CoreMinimal.h"
#include "Api/Auth/AuthApi.h"
#include "Api/Auth/IAuthenticationStrategy.h"

struct FRefreshTokenResponse;

class PLAYERZEROEDITOR_API DeveloperTokenAuthStrategy : public IAuthenticationStrategy
{
public:
	DeveloperTokenAuthStrategy();
	
	virtual void AddAuthToRequest(TSharedPtr<FApiRequest> ApiRequest) override;
	virtual void OnRefreshTokenResponse(TSharedPtr<FApiRequest>, const FRefreshTokenResponse& Response, bool bWasSuccessful) override;
	virtual void TryRefresh(TSharedPtr<FApiRequest> ApiRequest) override;
	
private:
	TSharedPtr<FAuthApi> AuthApi;
	TSharedPtr<FApiRequest> ApiRequestToRetry;
	void RefreshTokenAsync(const FRefreshTokenRequest& RefreshRequest);
};
