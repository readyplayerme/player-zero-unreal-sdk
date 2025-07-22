#include "Api/Blueprints/BlueprintApi.h"
#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

FBlueprintApi::FBlueprintApi()
{
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	BaseUrl = FString::Printf(TEXT("%s/v1/public/blueprints"), *PlayerZeroSettings->GetApiBaseUrl());
}

FBlueprintApi::~FBlueprintApi()
{
}

void FBlueprintApi::ListAsync(const FBlueprintListRequest& Request, FBlueprintApiListResponse OnComplete)
{
	const FString ArchivedStr = Request.Archived ? TEXT("true") : TEXT("false");
	TSharedPtr<FApiRequest> RequestPtr = MakeShared<FApiRequest>();
	RequestPtr->Url = FString::Printf(TEXT("%s?applicationId=%s&archived=%s"), *BaseUrl, *Request.ApplicationId, *ArchivedStr);
	RequestPtr->Method = GET;
	RequestPtr->Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	DispatchRaw(RequestPtr, FOnDispatchComplete::CreateLambda(
		[this, OnComplete](TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bSuccess)
		{
			if (bSuccess && Response.IsValid())
			{
				FBlueprintListResponse BlueprintListResponse;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &BlueprintListResponse, 0, 0))
				{
					OnComplete.ExecuteIfBound(BlueprintListResponse, true);
					return;
				}
			}
			UE_LOG(LogPlayerZero, Warning, TEXT("Blueprint LIST request failed."));
			OnComplete.ExecuteIfBound(FBlueprintListResponse(), false);
		}));
}
