#include "Api/Auth/ApiKeyAuthStrategy.h"

#include "PlayerZero.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

class UPlayerZeroSettings;

FApiKeyAuthStrategy::FApiKeyAuthStrategy()
{
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Auth/ApiKeyAuthStrategy.cpp
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
=======
	if(const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>())
	{
		ApiKey = RpmSettings->ApiKey;
	}
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Auth/ApiKeyAuthStrategy.cpp
}

FApiKeyAuthStrategy::FApiKeyAuthStrategy(const FString& InApiKey) : ApiKey(InApiKey)
{	
}
