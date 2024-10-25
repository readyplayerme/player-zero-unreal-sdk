#pragma once

#include "CoreMinimal.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Api/Auth/AuthApi.h"
#include "Api/Auth/IAuthenticationStrategy.h"

struct FRefreshTokenResponse;

class RPMNEXTGENEDITOR_API FDeveloperTokenAuthStrategy : public FApiKeyAuthStrategy, public FAuthApi
{
public:
	FDeveloperTokenAuthStrategy();
	FDeveloperTokenAuthStrategy(const FString& InApiKey);
	~FDeveloperTokenAuthStrategy();

	virtual void TryRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, const FRefreshTokenResponse&, bool)> OnTokenRefreshed) override;
	
private:
	TSharedPtr<FApiRequest> ApiRequestToRetry;
};
