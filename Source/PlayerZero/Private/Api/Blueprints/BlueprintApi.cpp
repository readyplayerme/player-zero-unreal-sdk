#include "Api/Blueprints/BlueprintApi.h"
#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

FBlueprintApi::FBlueprintApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	BaseUrl = FString::Printf(TEXT("%s/v1/blueprints"), *PlayerZeroSettings->GetApiBaseUrl());

	OnRequestComplete.BindRaw(this, &FBlueprintApi::HandleListResponse);
}

FBlueprintApi::~FBlueprintApi()
{
}

void FBlueprintApi::ListAsync(TSharedPtr<FBlueprintListRequest> Request, FBlueprintApiListResponse OnComplete)
{
	const FString ArchivedStr = Request->Archived ? TEXT("true") : TEXT("false");
	TSharedPtr<FApiRequest> RequestPtr = MakeShared<FApiRequest>();
	RequestPtr->Url = FString::Printf(TEXT("%s?applicationId=%s&archived=%s"), *BaseUrl, *Request->ApplicationId, *ArchivedStr);
	RequestPtr->Method = GET;
	RequestPtr->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	OnListResponse = OnComplete;
	DispatchRaw(RequestPtr);
}

void FBlueprintApi::HandleListResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TSharedPtr<FBlueprintListResponse> BlueprintListResponse;
		// TODO fix json convert
		// if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &BlueprintListResponse, 0, 0))
		// {
		// 	OnListResponse.ExecuteIfBound(BlueprintListResponse, true);
		// 	return;
		// }
	}
	
	UE_LOG(LogPlayerZero, Warning, TEXT("Blueprint LIST request failed."));
	OnListResponse.ExecuteIfBound(TSharedPtr<FBlueprintListResponse>(), false);
}
