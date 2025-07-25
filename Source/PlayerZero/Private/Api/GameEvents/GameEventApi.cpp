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

FGameEventApi::FGameEventApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	Url = FString::Printf(TEXT("%s/v1/public/events"), *PlayerZeroSettings->GetApiBaseUrl());
}

FGameEventApi::~FGameEventApi()
{
}

FString FGameEventApi::GetToken() const
{
	// TODO add logic for token retrieval
	return TEXT("YourTokenHere"); // Replace with actual logic
}

template<typename TEvent>
void FGameEventApi::SendGameEventAsync(const TEvent& Event, FOnGameEventSent OnComplete)
{
	FString Token = GetToken();
	
	TEvent PayloadWithToken = Event;
	if constexpr (HasToken_v<TEvent>)
	{
		PayloadWithToken.Token = Token;
	}
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