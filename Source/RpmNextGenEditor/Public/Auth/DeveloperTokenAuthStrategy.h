#pragma once

#include "CoreMinimal.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Api/Auth/AuthApi.h"
#include "Api/Auth/IAuthenticationStrategy.h"

struct FRefreshTokenResponse;

class RPMNEXTGENEDITOR_API FDeveloperTokenAuthStrategy : public IAuthenticationStrategy, public FAuthApi
{
public:
	FDeveloperTokenAuthStrategy();
	virtual ~FDeveloperTokenAuthStrategy() override;
	virtual void AddAuthToRequest(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, bool)> OnAuthenticationComplete) override;
	virtual void TryRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, const FRefreshTokenResponse&, bool)> OnTokenRefreshed) override;
	
private:
	TSharedPtr<FApiRequest> ApiRequestToRetry;
};
