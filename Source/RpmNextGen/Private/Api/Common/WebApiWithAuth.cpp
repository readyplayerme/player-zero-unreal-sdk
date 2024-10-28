#include "Api/Common/WebApiWithAuth.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Settings/RpmDeveloperSettings.h"

FWebApiWithAuth::FWebApiWithAuth() : AuthenticationStrategy(nullptr)
{
    FWebApi();
    const URpmDeveloperSettings* Settings = GetDefault<URpmDeveloperSettings>();
    if (!Settings->ApiKey.IsEmpty() || Settings->ApiProxyUrl.IsEmpty())
    {
        SetAuthenticationStrategy(MakeShared<FApiKeyAuthStrategy>());
    }
}

FWebApiWithAuth::FWebApiWithAuth(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy) : AuthenticationStrategy(InAuthenticationStrategy)
{
    FWebApi();
}

//TODO check if this is needed
FWebApiWithAuth::~FWebApiWithAuth()
{
    CancelAllRequests();
    AuthenticationStrategy = nullptr;
}

void FWebApiWithAuth::SetAuthenticationStrategy(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy)
{
    AuthenticationStrategy = InAuthenticationStrategy;
}
