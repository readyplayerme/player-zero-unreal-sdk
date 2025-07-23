#include "Api/Auth/AuthApi.h"

#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"
#include "Api/Auth/Models/RefreshTokenRequest.h"
#include "Api/Auth/Models/RefreshTokenResponse.h"
#include "Api/Common/WebApiHelpers.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

FAuthApi::FAuthApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	ApiUrl = FString::Printf(TEXT("%s/refresh"), *PlayerZeroSettings->ApiBaseAuthUrl);
}

void FAuthApi::RefreshToken(const FRefreshTokenRequest& Request, FOnRefreshTokenResponse OnComplete )
{
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = ApiUrl;
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = Request.ToJsonString();
	ApiRequest->OnApiRequestComplete = FOnApiRequestComplete::CreateLambda(
		[OnComplete](TSharedPtr<FApiRequest> Request, FHttpResponsePtr Response, bool bSuccess)
		{
			FRefreshTokenResponse ParsedResponse;
			if (bSuccess && TryParseJsonResponse(Response, ParsedResponse))
			{
				OnComplete.ExecuteIfBound(Request, ParsedResponse, true);
				return;
			}

			UE_LOG(LogPlayerZero, Warning, TEXT("Auth token refresh request failed."));
			OnComplete.ExecuteIfBound(Request, FRefreshTokenResponse(), false);
		});
	DispatchRaw(ApiRequest);
}
