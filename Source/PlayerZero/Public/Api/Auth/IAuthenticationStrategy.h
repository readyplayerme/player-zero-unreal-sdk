#pragma once

#include "CoreMinimal.h"
#include "Models/RefreshTokenResponse.h"

struct FApiRequest;

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Auth/IAuthenticationStrategy.h
DECLARE_DELEGATE_TwoParams(FOnAuthComplete, TSharedPtr<FApiRequest>, bool);
DECLARE_DELEGATE_ThreeParams(FOnTokenRefreshed, TSharedPtr<FApiRequest>, const FRefreshTokenResponseBody&, bool);

class PLAYERZERO_API IAuthenticationStrategy
=======
class RPMNEXTGEN_API IAuthenticationStrategy
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Auth/IAuthenticationStrategy.h
{
public:
	virtual ~IAuthenticationStrategy();
	virtual void AddAuthToRequest(TSharedPtr<FApiRequest> ApiRequest,  TFunction<void(TSharedPtr<FApiRequest>, bool)> OnAuthenticationComplete) = 0;

	virtual void TryRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, const FRefreshTokenResponse&, bool)> OnTokenRefreshed) = 0;
};

inline IAuthenticationStrategy::~IAuthenticationStrategy() = default;
