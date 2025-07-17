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
	OnRequestComplete.BindRaw(this, &FAuthApi::OnProcessComplete);
}

void FAuthApi::RefreshToken(const FRefreshTokenRequest& Request)
{
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = ApiUrl;
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = Request.ToJsonString();
	DispatchRaw(ApiRequest);
}

void FAuthApi::OnProcessComplete(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ApiRequest.IsValid())
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Invalid ApiRequest in FAuthApi::OnProcessComplete."));
		return;
	}

	if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FString Data = Response->GetContentAsString();
		FRefreshTokenResponse TokenResponse;

		if (!Data.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Data, &TokenResponse, 0, 0))
		{
			if (OnRefreshTokenResponse.IsBound())
			{
				OnRefreshTokenResponse.ExecuteIfBound(ApiRequest, TokenResponse, true);
			}
			return;
		}
	}

	UE_LOG(LogPlayerZero, Error, TEXT("Failed to refresh token"));
	if (OnRefreshTokenResponse.IsBound())
	{
		OnRefreshTokenResponse.ExecuteIfBound(ApiRequest, FRefreshTokenResponse(), false);
	}
}
