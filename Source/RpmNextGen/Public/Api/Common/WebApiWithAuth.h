#pragma once

#include "CoreMinimal.h"
#include "WebApi.h"
#include "Api/Auth/IAuthenticationStrategy.h"

class IAuthenticationStrategy;

class RPMNEXTGEN_API FWebApiWithAuth : public FWebApi
{
public:
    FWebApiWithAuth();
    FWebApiWithAuth(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy);
    virtual ~FWebApiWithAuth() override;

    virtual void SetAuthenticationStrategy(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy);

    template <typename T>
    void SendRequestWithAuth(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse);

private:
    TSharedPtr<IAuthenticationStrategy> AuthenticationStrategy;

    template <typename T>
    void SendAuthenticatedRequest(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse);

    template <typename T>
    void HandleFailedRequestWithRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse, int32 StatusCode);

    template <typename T>
    void RetryRequestWithRefreshedToken(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse);
};

template <typename T>
void FWebApiWithAuth::SendRequestWithAuth(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse)
{
    TWeakPtr<FWebApiWithAuth> WeakPtrThis =  StaticCastSharedRef<FWebApiWithAuth>(AsShared());

    if (AuthenticationStrategy)
    {
        AuthenticationStrategy->AddAuthToRequest(ApiRequest, [WeakPtrThis, ApiRequest, OnResponse](TSharedPtr<FApiRequest> AuthenticatedRequest, bool bAuthSuccess)
        {
            if (WeakPtrThis.IsValid())
            {
                WeakPtrThis.Pin()->SendAuthenticatedRequest<T>(AuthenticatedRequest, OnResponse);
            }
            else
            {
                OnResponse(nullptr, false, -1);
            }
        });
        return;
    }
    SendRequest<T>(ApiRequest, OnResponse);
}

template <typename T>
void FWebApiWithAuth::SendAuthenticatedRequest(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse)
{
    TWeakPtr<FWebApiWithAuth> WeakPtrThis = StaticCastSharedRef<FWebApiWithAuth>(AsShared());

    SendRequest<T>(ApiRequest, [WeakPtrThis, ApiRequest, OnResponse](TSharedPtr<T> Response, bool bWasSuccessful, int32 StatusCode)
    {
        if (WeakPtrThis.IsValid())
        {
            if (StatusCode == 401)
            {
                WeakPtrThis.Pin()->HandleFailedRequestWithRefresh<T>(ApiRequest, OnResponse, StatusCode);
            }
            else
            {
                OnResponse(Response, bWasSuccessful, StatusCode);
            }
        }
    });
}

template <typename T>
void FWebApiWithAuth::HandleFailedRequestWithRefresh(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse, int32 StatusCode)
{
    TWeakPtr<FWebApiWithAuth> WeakPtrThis = StaticCastSharedRef<FWebApiWithAuth>(AsShared());

    AuthenticationStrategy->TryRefresh(ApiRequest, [WeakPtrThis, ApiRequest, OnResponse, StatusCode](TSharedPtr<FApiRequest> RefreshedRequest, const FRefreshTokenResponse& RefreshResponse, bool bRefreshSuccess)
    {
        if (WeakPtrThis.IsValid())
        {
            if (bRefreshSuccess)
            {
                WeakPtrThis.Pin()->RetryRequestWithRefreshedToken<T>(RefreshedRequest, OnResponse);
            }
            else
            {
                OnResponse(nullptr, false, StatusCode);
            }
        }
    });
}

template <typename T>
void FWebApiWithAuth::RetryRequestWithRefreshedToken(TSharedPtr<FApiRequest> ApiRequest, TFunction<void(TSharedPtr<T>, bool, int32)> OnResponse)
{
    TWeakPtr<FWebApiWithAuth> WeakPtrThis = StaticCastSharedRef<FWebApiWithAuth>(AsShared());

    SendRequest<T>(ApiRequest, [WeakPtrThis, OnResponse](TSharedPtr<T> Response, bool bWasSuccessful, int32 StatusCode)
    {
        if (WeakPtrThis.IsValid())
        {
            OnResponse(Response, bWasSuccessful, StatusCode);
        }
    });
}
