<<<<<<< HEAD:Source/PlayerZero/Private/Api/Auth/AuthApi.cpp
﻿#include "Api/Auth/AuthApi.h"

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
=======
﻿
#include "Api/Auth/AuthApi.h"
#include "Api/Auth/Models/CreateUserResponse.h"
#include "Api/Auth/Models/LoginWithCodeResponse.h"
#include "Api/Auth/Models/RefreshTokenResponse.h"
#include "Settings/RpmDeveloperSettings.h"
// ReSharper disable All
#include "Api/Auth/Models/RefreshTokenRequest.h"
#include "Api/Auth/Models/SendLoginCodeRequest.h"
#include "Api/Auth/Models/LoginWithCodeRequest.h"
#include "Api/Auth/Models/CreateUserRequest.h"
// ReSharper restore All

FAuthApi::FAuthApi()
{
	RpmSettings = GetDefault<URpmDeveloperSettings>();
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Auth/AuthApi.cpp
}

void FAuthApi::RefreshToken(const FRefreshTokenRequest& Request, FOnRefreshTokenResponse OnComplete)
{
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/refresh"), *RpmSettings->ApiBaseAuthUrl);
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = ConvertToJsonString(Request);
	SendRequest<FRefreshTokenResponse>(ApiRequest, [OnComplete](TSharedPtr<FRefreshTokenResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
}

void FAuthApi::SendLoginCode(const FSendLoginCodeRequest& Request, FOnSendLoginCodeResponse OnComplete)
{
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/v1/auth/request-login-code"), *RpmSettings->GetApiBaseUrl());
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = ConvertToJsonString(Request);
	// TODO confirm this works since this request has no return data
	SendRequest<FApiResponse>(ApiRequest, [OnComplete](TSharedPtr<FApiResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Auth/AuthApi.cpp
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
=======
		OnComplete.ExecuteIfBound(bWasSuccessful && Response.IsValid());
	});
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
