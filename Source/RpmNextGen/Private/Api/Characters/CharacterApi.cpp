#include "Api/Characters/CharacterApi.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Api/Characters/Models/CharacterFindByIdRequest.h"
#include "Settings/RpmDeveloperSettings.h"

FCharacterApi::FCharacterApi()
{
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	BaseUrl = FString::Printf(TEXT("%s/v1/characters"), *RpmSettings->GetApiBaseUrl());
}

FCharacterApi::~FCharacterApi()
{
}

void FCharacterApi::FindByIdAsync(TSharedPtr<FCharacterFindByIdRequest> Request, FOnCharacterFindResponse OnComplete)
{
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/%s"), *BaseUrl, *Request->Id);
	ApiRequest->Method = GET;
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	SendRequestWithAuth<FCharacterFindByIdResponse>(ApiRequest, [OnComplete](TSharedPtr<FCharacterFindByIdResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		if (StatusCode == 401)
		{
			UE_LOG(LogReadyPlayerMe, Error,TEXT("The request to the character API failed with a 401 response code. Please ensure that your API Key or proxy is correctly configured."));
		}
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
}
