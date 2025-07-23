#include "Api/GameEvents/GameEventApi.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "PlayerZero.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

class UPlayerZeroDeveloperSettings;

UGameEventApi::UGameEventApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	Url = FString::Printf(TEXT("%s/v1/public/events"), *PlayerZeroSettings->GetApiBaseUrl());
}

UGameEventApi::~UGameEventApi()
{
}

FString UGameEventApi::GetToken() const
{
	// This mimics ZeroQueryParams.GetParams().TryGetValue("token")
	return TEXT("YourTokenHere"); // Replace with actual logic
}

template<typename TPayload>
void UGameEventApi::SendGameEventAsync(const TPayload& RequestPayload, FOnGameEventSent OnComplete)
{
	FString Token = GetToken();
	
	const TPayload PayloadWithToken = RequestPayload;
	PayloadWithToken.Token = Token;
	FString JsonPayload;
	if (!FJsonObjectConverter::UStructToJsonObjectString(PayloadWithToken, JsonPayload))
	{
		OnComplete.ExecuteIfBound(false, TEXT("Failed to serialize JSON"));
		return;
	}
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Url;
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = JsonPayload;
	ApiRequest->OnApiRequestComplete = FOnApiRequestComplete::CreateLambda(
		[OnComplete](TSharedPtr<FApiRequest> Req, FHttpResponsePtr Response, bool bSuccess)
		{
			if (bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				OnComplete.ExecuteIfBound(true, Response->GetContentAsString());
			}
			else
			{
				FString ErrorMsg = Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed");
				OnComplete.ExecuteIfBound(false, ErrorMsg);
			}
		});
	DispatchRaw(ApiRequest);
}