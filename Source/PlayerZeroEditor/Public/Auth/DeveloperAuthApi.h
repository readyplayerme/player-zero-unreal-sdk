#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"

struct FDeveloperLoginResponse;
struct FDeveloperLoginRequest;

DECLARE_DELEGATE_TwoParams(FOnDeveloperLoginResponse, const FDeveloperLoginResponse&, bool);

class PLAYERZEROEDITOR_API FDeveloperAuthApi : public FWebApi
{
public:
	//FOnDeveloperLoginResponse OnLoginResponse;
	
	FDeveloperAuthApi();

	void HandleLoginResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful) const;
	void LoginWithEmail(FDeveloperLoginRequest Request, FOnDeveloperLoginResponse OnComplete);

private:
	FString ApiUrl;
};
