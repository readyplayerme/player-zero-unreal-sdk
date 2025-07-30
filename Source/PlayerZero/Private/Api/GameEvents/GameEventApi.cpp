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
#include "Utilities/JsonHelper.h"

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
	return TEXT(""); // Replace with actual logic
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
	if (!FJsonHelper::StructToCleanJsonString(PayloadWithToken, JsonPayload))
	{
		OnComplete.ExecuteIfBound(false, TEXT("Failed to serialize JSON"));
		return;
	}
	
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = Url;
	ApiRequest->Method = POST;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	ApiRequest->Payload = JsonPayload;
	TSharedPtr<FGameEventApi> SharedThis = StaticCastSharedRef<FGameEventApi>(AsShared());
	ApiRequest->OnApiRequestComplete = FOnApiRequestComplete::CreateLambda(
		[SharedThis, OnComplete](TSharedPtr<FApiRequest> Request, FHttpResponsePtr Response, bool bSuccess)
		{
			if (!SharedThis.IsValid())
			{
				return;
			}
			// print request URL and payload for debugging

			if (bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				UE_LOG(LogPlayerZero, Log, TEXT("GameEventApi SUCCESS Request URL: %s"), *Request->Url);
				UE_LOG(LogPlayerZero, Log, TEXT("Game EventApi SUCCESS Request Payload: %s"), *Request->Payload);
				OnComplete.ExecuteIfBound(true, Response->GetContentAsString());
			}
			else
			{
				UE_LOG(LogPlayerZero, Log, TEXT("GameEventApi FAIL Request URL: %s"), *Request->Url);
				UE_LOG(LogPlayerZero, Log, TEXT("Game EventApi FAIL Request Payload: %s"), *Request->Payload);
				FString ErrorMsg = Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed");
				OnComplete.ExecuteIfBound(false, ErrorMsg);
			}
		});
	DispatchRaw(ApiRequest);
}