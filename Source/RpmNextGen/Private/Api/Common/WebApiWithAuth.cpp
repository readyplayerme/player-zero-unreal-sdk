#include "Api/Common/WebApiWithAuth.h"
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Settings/RpmDeveloperSettings.h"

FWebApiWithAuth::FWebApiWithAuth() : AuthenticationStrategy(nullptr)
{
    FWebApi();
    const URpmDeveloperSettings* Settings = GetDefault<URpmDeveloperSettings>();
    if (Settings->ApplicationId.IsEmpty())
    {
        UE_LOG(LogReadyPlayerMe, Error, TEXT("Application ID is empty. Please set the Application ID in the Ready Player Me Developer Settings"));
    }
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
