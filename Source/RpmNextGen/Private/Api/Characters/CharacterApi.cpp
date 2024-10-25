#include "Api/Characters/CharacterApi.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Api/Characters/Models/CharacterFindByIdRequest.h"
#include "Api/Characters/Models/CharacterPreviewRequest.h"
#include "Api/Characters/Models/CharacterUpdateRequest.h"
#include "Settings/RpmDeveloperSettings.h"

FCharacterApi::FCharacterApi()
{
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	BaseUrl = FString::Printf(TEXT("%s/v1/characters"), *RpmSettings->GetApiBaseUrl());
}

FCharacterApi::~FCharacterApi()
{
}

void FCharacterApi::CreateAsync(TSharedPtr<FCharacterCreateRequest> Request, FOnCharacterCreateResponse OnComplete)
{
	AssetByType.Append(Request->Data.Assets);
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s"), *BaseUrl);
	ApiRequest->Method = POST;
	ApiRequest->Payload = ConvertToJsonString(Request);
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	SendRequestWithAuth<FCharacterCreateResponse>(ApiRequest, [OnComplete](TSharedPtr<FCharacterCreateResponse> Response, bool bWasSuccessful, int32 StatusCode)
	 {
		if (StatusCode == 401)
		{
			UE_LOG(LogReadyPlayerMe, Error,TEXT("The request to the character API failed with a 401 response code. Please ensure that your API Key or proxy is correctly configured."));
		}
		 OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	 });
}

void FCharacterApi::UpdateAsync(TSharedPtr<FCharacterUpdateRequest> Request, FOnCharacterUpdatResponse OnComplete)
{
	AssetByType.Append(Request->Payload.Assets);
	TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
	ApiRequest->Url = FString::Printf(TEXT("%s/%s"), *BaseUrl, *Request->Id);
	ApiRequest->Method = PATCH;
	ApiRequest->Payload = ConvertToJsonString(Request);
	ApiRequest->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	SendRequestWithAuth<FCharacterUpdateResponse>(ApiRequest, [OnComplete](TSharedPtr<FCharacterUpdateResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		if (StatusCode == 401)
		{
			UE_LOG(LogReadyPlayerMe, Error,TEXT("The request to the character API failed with a 401 response code. Please ensure that your API Key or proxy is correctly configured."));
		}
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
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

FString FCharacterApi::GeneratePreviewUrl(const FCharacterPreviewRequest& Request)
{
	FString QueryString = BuildQueryString(Request.Params.Assets);
	FString url = FString::Printf(TEXT("%s/%s/preview%s"), *BaseUrl, *Request.Id, *QueryString);
	return url;
}
