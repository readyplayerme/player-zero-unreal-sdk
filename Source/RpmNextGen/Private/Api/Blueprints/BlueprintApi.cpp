#include "Api/Blueprints/BlueprintApi.h"
#include "Settings/RpmDeveloperSettings.h"

FBlueprintApi::FBlueprintApi()
{
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	BaseUrl = FString::Printf(TEXT("%s/v1/blueprints"), *RpmSettings->GetApiBaseUrl());
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
	SendRequest<FBlueprintListResponse>(RequestPtr, [OnComplete](TSharedPtr<FBlueprintListResponse> Response, bool bWasSuccessful, int32 StatusCode)
	{
		if (StatusCode == 401)
		{
			UE_LOG(LogReadyPlayerMe, Error, TEXT("The request to the blueprint API failed with a 401 response code. Please ensure that your API Key or proxy is correctly configured."));
		}
		OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
	});
}
