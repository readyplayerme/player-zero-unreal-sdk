#include "Api/GameEvents/GameEventApi.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "PlayerZero.h"
#include "Api/GameEvents/GameEventTypes.h"
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
	return TEXT(""); // Replace with actual logic
}

template<typename TProps>
void FGameEventApi::SendGameEventAsync(const TGameEventWrapper<TProps>& Wrapper, FOnGameEventSent OnComplete)
{
	TSharedPtr<FJsonObject> WrappedPayload = Wrapper.ToWrappedJson();

	FString JsonPayload;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonPayload);
	if (!FJsonSerializer::Serialize(WrappedPayload.ToSharedRef(), Writer))
	{
		OnComplete.ExecuteIfBound(false, TEXT("Failed to serialize wrapped JSON"));
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
			if (!SharedThis.IsValid()) return;
			UE_LOG(LogPlayerZero, Log , TEXT("GameEventApi Request event Payload: %s"), *Request->Payload);
			if (bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				UE_LOG(LogPlayerZero, Log, TEXT("GameEventApi SUCCESS Payload: %s"), *Request->Payload);
				OnComplete.ExecuteIfBound(true, Response->GetContentAsString());
			}
			else
			{
				FString ErrorMsg = Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed");
				UE_LOG(LogPlayerZero, Warning, TEXT("GameEventApi FAIL: %s Payload: %s"), *ErrorMsg , *Request->Payload);
				OnComplete.ExecuteIfBound(false, ErrorMsg);
			}
		});
	DispatchRaw(ApiRequest);
}