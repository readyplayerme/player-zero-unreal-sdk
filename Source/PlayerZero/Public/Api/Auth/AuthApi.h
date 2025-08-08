#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApiWithAuth.h"

struct FApiResponse;
struct FCreateUserResponse;
struct FLoginWithCodeResponse;
class URpmDeveloperSettings;
struct FCreateUserRequest;
struct FLoginWithCodeRequest;
struct FSendLoginCodeRequest;
struct FRefreshTokenResponse;
struct FRefreshTokenRequest;

DECLARE_DELEGATE_TwoParams(FOnRefreshTokenResponse, TSharedPtr<FRefreshTokenResponse>, bool);
DECLARE_DELEGATE_OneParam(FOnSendLoginCodeResponse, bool);
DECLARE_DELEGATE_TwoParams(FOnLoginWithCodeResponse, TSharedPtr<const FLoginWithCodeResponse>, bool);
DECLARE_DELEGATE_TwoParams(FOnCreateUserResponse, TSharedPtr<FCreateUserResponse>, bool);

class PLAYERZERO_API FAuthApi : public FWebApi
{
public:
	
	FAuthApi();
	void RefreshToken(const FRefreshTokenRequest& Request, FOnRefreshTokenResponse OnComplete);

private:
	const URpmDeveloperSettings* RpmSettings;
};
