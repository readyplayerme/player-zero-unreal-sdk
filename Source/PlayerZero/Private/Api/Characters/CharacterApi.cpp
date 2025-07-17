#include "Api/Characters/CharacterApi.h"
#include "HttpModule.h"
#include "PlayerZero.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Api/Characters/Models/CharacterFindByIdRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

FCharacterApi::FCharacterApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	BaseUrl = FString::Printf(TEXT("%s/v1/characters"), *PlayerZeroSettings->GetApiBaseUrl());
	if(!PlayerZeroSettings->ApiKey.IsEmpty() && PlayerZeroSettings->ApiProxyUrl.IsEmpty())
	{
		SetAuthenticationStrategy(MakeShared<FApiKeyAuthStrategy>());
	}
	OnRequestComplete.BindRaw(this, &FCharacterApi::HandleCharacterResponse);
}

FCharacterApi::~FCharacterApi()
{
}

void FCharacterApi::FindByIdAsync(const FCharacterFindByIdRequest& Request)
{
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/%s"), *BaseUrl, *Request.Id);
	ApiRequest->Method = GET;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	DispatchRawWithAuth(ApiRequest);
}


void FCharacterApi::HandleCharacterResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful)
{
	const FString Verb = ApiRequest->GetVerb();
	bool bSuccess = bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
	if (Response.IsValid() && Response->GetResponseCode() == 401)
	{
		UE_LOG(LogPlayerZero, Error,TEXT("The request to the character API failed with a 401 response code. Please ensure that your API Key or proxy is correctly configured."));
		bSuccess = false;
	}
	switch (ApiRequest->Method)
	{
		case GET: 
			HandleFindResponse(Response, bSuccess);
			break;
		default:
			break;
	}
}

void FCharacterApi::HandleFindResponse(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		FCharacterFindByIdResponse CharacterFindByIdResponse;
		if(FJsonObjectConverter::JsonObjectStringToUStruct(	Response->GetContentAsString(), &CharacterFindByIdResponse, 0, 0))
		{
			OnCharacterFindResponse.ExecuteIfBound(CharacterFindByIdResponse, true);
			return;
		}
	}
	UE_LOG(LogPlayerZero, Warning, TEXT("Character FIND request failed."));
	OnCharacterFindResponse.ExecuteIfBound(FCharacterFindByIdResponse(), false);
}



