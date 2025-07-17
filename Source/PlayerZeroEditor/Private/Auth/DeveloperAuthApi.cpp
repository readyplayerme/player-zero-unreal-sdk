#include "Auth/DeveloperAuthApi.h"
#include "Auth/Models/DeveloperLoginRequest.h"
#include "Auth/Models/DeveloperLoginResponse.h"
<<<<<<< HEAD:Source/PlayerZeroEditor/Private/Auth/DeveloperAuthApi.cpp
#include "Interfaces/IHttpResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

FDeveloperAuthApi::FDeveloperAuthApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	ApiUrl = FString::Printf(TEXT("%s/login"), *PlayerZeroSettings->ApiBaseAuthUrl);
	OnRequestComplete.BindRaw(this, &FDeveloperAuthApi::HandleLoginResponse);
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

void FDeveloperAuthApi::LoginWithEmail(FDeveloperLoginRequest Request)
=======
#include "Settings/RpmDeveloperSettings.h"

FDeveloperAuthApi::FDeveloperAuthApi()
{
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	ApiUrl = FString::Printf(TEXT("%s/login"), *RpmSettings->ApiBaseAuthUrl);
}

void FDeveloperAuthApi::LoginWithEmail(TSharedPtr<FDeveloperLoginRequest> Request, FOnDeveloperLoginResponse OnComplete)
>>>>>>> origin/develop:Source/RpmNextGenEditor/Private/Auth/DeveloperAuthApi.cpp
{
	const TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = ApiUrl;
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = ConvertToJsonString(*Request.Get());
	SendRequest<FDeveloperLoginResponse>(ApiRequest, [OnComplete](TSharedPtr<FDeveloperLoginResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});

}
