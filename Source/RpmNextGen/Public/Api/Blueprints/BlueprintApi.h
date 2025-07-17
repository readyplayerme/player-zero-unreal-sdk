#pragma once
#include "Api/Common/WebApi.h"
#include "Models/BlueprintListRequest.h"
#include "Models/BlueprintListResponse.h"

DECLARE_DELEGATE_TwoParams(FBlueprintApiListResponse, TSharedPtr<FBlueprintListResponse>, bool);

class RPMNEXTGEN_API FBlueprintApi : public FWebApi
{
public:

	FBlueprintApi();
	virtual ~FBlueprintApi() override;

	void ListAsync(TSharedPtr<FBlueprintListRequest> Request, FBlueprintApiListResponse OnComplete);

private:
	FString BaseUrl;
};
