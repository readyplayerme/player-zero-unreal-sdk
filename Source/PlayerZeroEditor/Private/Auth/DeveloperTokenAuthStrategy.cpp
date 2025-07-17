#include "Auth/DeveloperTokenAuthStrategy.h"
<<<<<<< HEAD:Source/PlayerZeroEditor/Private/Auth/DeveloperTokenAuthStrategy.cpp
#include "PlayerZero.h"
=======
>>>>>>> origin/develop:Source/RpmNextGenEditor/Private/Auth/DeveloperTokenAuthStrategy.cpp
#include "Auth/DevAuthTokenCache.h"
#include "Api/Auth/Models/RefreshTokenRequest.h"
#include "Api/Auth/Models/RefreshTokenResponse.h"
#include "Auth/Models/DeveloperAuth.h"

FDeveloperTokenAuthStrategy::FDeveloperTokenAuthStrategy()
{
}

FDeveloperTokenAuthStrategy::~FDeveloperTokenAuthStrategy()
{
	CancelAllRequests();
}

void FDeveloperTokenAuthStrategy::AddAuthToRequest(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<FApiRequest>, bool)> OnAuthenticationComplete)
{
	FDeveloperAuth DeveloperAuth = FDevAuthTokenCache::GetAuthData();
	if (DeveloperAuth.Token.IsEmpty())
	{
<<<<<<< HEAD:Source/PlayerZeroEditor/Private/Auth/DeveloperTokenAuthStrategy.cpp
		UE_LOG(LogPlayerZero, Error, TEXT("Token is empty"));
		OnAuthComplete.ExecuteIfBound(ApiRequest, false);
=======
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Developer Token not set!"));
		OnAuthenticationComplete(ApiRequest, false);
>>>>>>> origin/develop:Source/RpmNextGenEditor/Private/Auth/DeveloperTokenAuthStrategy.cpp
		return;
	}
	ApiRequest->Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *DeveloperAuth.Token));

	OnAuthenticationComplete(ApiRequest, true);
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

