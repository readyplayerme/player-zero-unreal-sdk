#include "Api/Auth/ApiKeyAuthStrategy.h"

#include "PlayerZero.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

class UPlayerZeroSettings;

FApiKeyAuthStrategy::FApiKeyAuthStrategy()
{
}

void FApiKeyAuthStrategy::AddAuthToRequest(TSharedPtr<FApiRequest> ApiRequest)
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	if(PlayerZeroSettings->ApiKey.IsEmpty())
	{
		UE_LOG(LogPlayerZero, Error, TEXT("API Key is empty"));
		OnAuthComplete.ExecuteIfBound(ApiRequest, false);
		return;
	}
	ApiRequest->Headers.Add(TEXT("X-API-KEY"), PlayerZeroSettings->ApiKey);
	OnAuthComplete.ExecuteIfBound(ApiRequest, true);
}

void FApiKeyAuthStrategy::OnRefreshTokenResponse(TSharedPtr<FApiRequest> ApiRequest, const FRefreshTokenResponse& Response, bool bWasSuccessful)
{
	OnTokenRefreshed.ExecuteIfBound(ApiRequest, Response.Data, bWasSuccessful);
}

void FApiKeyAuthStrategy::TryRefresh(TSharedPtr<FApiRequest> ApiRequest)
{
	OnAuthComplete.ExecuteIfBound(ApiRequest, false);
}
