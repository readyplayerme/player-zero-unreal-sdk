#include "Api/Auth/AuthApi.h"

#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"
#include "Api/Auth/Models/RefreshTokenRequest.h"
#include "Api/Auth/Models/RefreshTokenResponse.h"
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
	DispatchRaw(ApiRequest, FOnDispatchComplete::CreateLambda(
		[OnComplete](TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bSuccess)
		{
			if (bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				FRefreshTokenResponse TokenResponse;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &TokenResponse, 0, 0))
				{
					OnComplete.ExecuteIfBound(ApiRequest, TokenResponse, true);
					return;
				}
			}
			UE_LOG(LogPlayerZero, Warning, TEXT("Auth token refresh request failed."));
			OnComplete.ExecuteIfBound(ApiRequest, FRefreshTokenResponse(), false);
		}));
}
