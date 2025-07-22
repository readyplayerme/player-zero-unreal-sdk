#include "Auth/DeveloperAuthApi.h"
#include "Auth/Models/DeveloperLoginRequest.h"
#include "Auth/Models/DeveloperLoginResponse.h"
#include "Interfaces/IHttpResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

FDeveloperAuthApi::FDeveloperAuthApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	ApiUrl = FString::Printf(TEXT("%s/login"), *PlayerZeroSettings->ApiBaseAuthUrl);
}

void FDeveloperAuthApi::HandleLoginResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful) const
{
	FDeveloperLoginResponse DevLoginResponse;
	if(bWasSuccessful && Response.IsValid())
	{
		const FString Data = Response->GetContentAsString();
		if (!Data.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Data, &DevLoginResponse, 0, 0))
		{
			OnLoginResponse.ExecuteIfBound(DevLoginResponse, true);
			return;
		}
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to parse login response: %s"), *Data );
	}

	OnLoginResponse.ExecuteIfBound(DevLoginResponse, bWasSuccessful);
}

void FDeveloperAuthApi::LoginWithEmail(FDeveloperLoginRequest Request, FOnDeveloperLoginResponse OnComplete)
{
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = ApiUrl;
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = Request.ToJsonString();
	TSharedPtr<FOnDispatchComplete> OnRequestComplete = MakeShared<FOnDispatchComplete>();
	OnRequestComplete->BindLambda(
	[this, OnComplete](TSharedPtr<FApiRequest> Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		FDeveloperLoginResponse DevLoginResponse;
		if(bWasSuccessful && Response.IsValid())
		{
			const FString Data = Response->GetContentAsString();
			if (!Data.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Data, &DevLoginResponse, 0, 0))
			{
				OnComplete.ExecuteIfBound(DevLoginResponse, true);
				return;
			}
			UE_LOG(LogPlayerZero, Error, TEXT("Failed to parse login response: %s"), *Data );
		}
		OnComplete.ExecuteIfBound(DevLoginResponse, bWasSuccessful);
	});
	DispatchRaw(ApiRequest, OnRequestComplete);
}
