<<<<<<< HEAD:Source/PlayerZero/Private/Api/Auth/AuthApi.cpp
﻿#include "Api/Auth/AuthApi.h"

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
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/refresh"), *RpmSettings->ApiBaseAuthUrl);
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = Request.ToJsonString();
	TSharedPtr<FAuthApi> SharedThis = StaticCastSharedRef<FAuthApi>(AsShared());
	ApiRequest->OnApiRequestComplete = FOnApiRequestComplete::CreateLambda(
		[SharedThis, OnComplete](TSharedPtr<FApiRequest> Request, FHttpResponsePtr Response, bool bSuccess)
		{
			if (SharedThis.IsValid() == false)
			{
				UE_LOG(LogPlayerZero, Warning, TEXT("FAuthApi is no longer valid when processing HTTP response to URL: %s"), *Request->Url);
				return;
			}
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

void FAuthApi::LoginWithCode(const FLoginWithCodeRequest& Request, FOnLoginWithCodeResponse OnComplete)
{
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/v1/auth/login"), *RpmSettings->GetApiBaseUrl());
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = ConvertToJsonString(Request);
	SendRequest<FLoginWithCodeResponse>(ApiRequest, [OnComplete](TSharedPtr<FLoginWithCodeResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
}

void FAuthApi::CreateUser(const FCreateUserRequest& Request, FOnCreateUserResponse OnComplete)
{
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/v1/users"), *RpmSettings->GetApiBaseUrl());
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = ConvertToJsonString(Request);
	SendRequest<FCreateUserResponse>(ApiRequest, [OnComplete](TSharedPtr<FCreateUserResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Auth/AuthApi.cpp
}
