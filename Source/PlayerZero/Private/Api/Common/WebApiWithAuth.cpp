#include "Api/Common/WebApiWithAuth.h"
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Common/WebApiWithAuth.cpp
#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"
=======
#include "Api/Auth/ApiKeyAuthStrategy.h"
#include "Settings/RpmDeveloperSettings.h"
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Common/WebApiWithAuth.cpp

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
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Common/WebApiWithAuth.cpp
    if (AuthenticationStrategy.IsValid())
    {
        AuthenticationStrategy->OnAuthComplete.Unbind();
        AuthenticationStrategy->OnTokenRefreshed.Unbind();
        
        AuthenticationStrategy->OnAuthComplete.BindRaw(this, &FWebApiWithAuth::OnAuthComplete);
        AuthenticationStrategy->OnTokenRefreshed.BindRaw(this, &FWebApiWithAuth::OnAuthTokenRefreshed);
    }
}

void FWebApiWithAuth::OnAuthComplete(TSharedPtr<FApiRequest> ApiRequest, bool bWasSuccessful)
{
    if (bWasSuccessful && ApiRequest.IsValid())
    {
        DispatchRaw(ApiRequest);
    }
    else if (ApiRequest.IsValid())
    {
        ApiRequest->OnApiRequestComplete.ExecuteIfBound(ApiRequest, FHttpResponsePtr(), false);
    }
}

void FWebApiWithAuth::OnAuthTokenRefreshed(TSharedPtr<FApiRequest> ApiRequest, const FRefreshTokenResponseBody& Response, bool bWasSuccessful)
{
    if (bWasSuccessful && ApiRequest.IsValid())
    {
        ApiRequest->Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Response.Token));
        DispatchRaw(ApiRequest);
    }
    else if (ApiRequest.IsValid())
    {
        ApiRequest->OnApiRequestComplete.ExecuteIfBound(ApiRequest, FHttpResponsePtr(), false);
    }
}

void FWebApiWithAuth::DispatchRawWithAuth(TSharedPtr<FApiRequest> ApiRequest)
{
    if (AuthenticationStrategy == nullptr)
    {
        DispatchRaw(ApiRequest);
        return;
    }

    AuthenticationStrategy->AddAuthToRequest(ApiRequest);
}

void FWebApiWithAuth::OnProcessResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TSharedPtr<FApiRequest> ApiRequest)
{
    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        ApiRequest->OnApiRequestComplete.ExecuteIfBound(ApiRequest, Response, true);
    }
    else if (Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Denied && AuthenticationStrategy != nullptr)
    {
        AuthenticationStrategy->TryRefresh(ApiRequest);
    }
    else
    {
        UE_LOG(LogPlayerZero, Warning, TEXT("WebApiWithAuth: Request to %s failed"), *Request->GetURL());
        ApiRequest->OnApiRequestComplete.ExecuteIfBound(ApiRequest, Response, false);
    }
=======
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Common/WebApiWithAuth.cpp
}
