#include "Auth/DeveloperTokenAuthStrategy.h"
#include "Auth/DevAuthTokenCache.h"
#include "Api/Auth/Models/RefreshTokenRequest.h"
#include "Api/Auth/Models/RefreshTokenResponse.h"
#include "Auth/Models/DeveloperAuth.h"

FDeveloperTokenAuthStrategy::FDeveloperTokenAuthStrategy()
{
}

FDeveloperTokenAuthStrategy::FDeveloperTokenAuthStrategy(const FString& InApiKey) : FApiKeyAuthStrategy(InApiKey)
{
}

FDeveloperTokenAuthStrategy::~FDeveloperTokenAuthStrategy()
{
	CancelAllRequests();
}

void FDeveloperTokenAuthStrategy::TryRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, const FRefreshTokenResponse&, bool)> OnTokenRefreshed)
{
	FRefreshTokenRequest RefreshRequest;
	RefreshRequest.Data.Token = FDevAuthTokenCache::GetAuthData().Token;
	RefreshRequest.Data.RefreshToken = FDevAuthTokenCache::GetAuthData().RefreshToken;
    
	TWeakPtr<FAuthApi> WeakPtrThis = SharedThis(this);

	RefreshToken(RefreshRequest, FOnRefreshTokenResponse::CreateLambda([WeakPtrThis, ApiRequest, OnTokenRefreshed](TSharedPtr<FRefreshTokenResponse> Response, bool bWasSuccessful)
	{
		if (WeakPtrThis.IsValid())
		{
			if (bWasSuccessful && !Response->Data.Token.IsEmpty())
			{
				FDeveloperAuth DeveloperAuth = FDevAuthTokenCache::GetAuthData();
				DeveloperAuth.Token = Response->Data.Token;
				DeveloperAuth.RefreshToken = Response->Data.RefreshToken;
				FDevAuthTokenCache::SetAuthData(DeveloperAuth);
                
				OnTokenRefreshed(ApiRequest, *Response, true);
				return;
			}
			OnTokenRefreshed(ApiRequest, *Response, false);
		}
	}));
}

