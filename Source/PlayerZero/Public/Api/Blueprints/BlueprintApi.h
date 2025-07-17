#pragma once
#include "Api/Common/WebApi.h"
#include "Models/BlueprintListRequest.h"
#include "Models/BlueprintListResponse.h"

DECLARE_DELEGATE_TwoParams(FBlueprintApiListResponse, const FBlueprintListResponse&, bool);

class PLAYERZERO_API FBlueprintApi : public FWebApi
{
public:
	FBlueprintApiListResponse OnListResponse;

	FBlueprintApi();
	virtual ~FBlueprintApi() override;

	void ListAsync(const FBlueprintListRequest& Request);

	void HandleListResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FString BaseUrl;
};
