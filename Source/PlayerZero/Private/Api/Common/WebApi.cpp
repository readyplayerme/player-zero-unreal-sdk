#include "Api/Common/WebApi.h"
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Common/WebApi.cpp
#include "HttpModule.h"
#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"
=======
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Common/WebApi.cpp

FWebApi::FWebApi()
{
    HttpModule = &FHttpModule::Get();
}

FWebApi::~FWebApi()
{
    CancelAllRequests();
}

void FWebApi::CancelAllRequests()
{
    for (const auto& Request : ActiveRequests)
    {
        Request->OnProcessRequestComplete().Unbind();
        Request->CancelRequest();
    }
<<<<<<< HEAD:Source/PlayerZero/Private/Api/Common/WebApi.cpp

    if (!ApiRequest->Payload.IsEmpty() && ApiRequest->Method != ERequestMethod::GET)
    {
        Request->SetContentAsString(ApiRequest->Payload);
    }
    Request->OnProcessRequestComplete().BindRaw(this, &FWebApi::OnProcessResponse, ApiRequest);
    Request->ProcessRequest();
}

void FWebApi::OnProcessResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TSharedPtr<FApiRequest> ApiRequest)
{
    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        OnRequestComplete.ExecuteIfBound(ApiRequest, Response, true);
        return;
    }
    FString ErrorMessage = Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed");
    UE_LOG(LogPlayerZero, Warning, TEXT("WebApi from URL %s request failed: %s"), *Request->GetURL(), *ErrorMessage);
    OnRequestComplete.ExecuteIfBound(ApiRequest, Response, false);
=======
    ActiveRequests.Empty();
>>>>>>> origin/develop:Source/RpmNextGen/Private/Api/Common/WebApi.cpp
}

FString FWebApi::BuildQueryString(const TMap<FString, FString>& QueryParams)
{
    FString QueryString;
    if (QueryParams.Num() > 0)
    {
        QueryString.Append(TEXT("?"));
        for (const auto& Param : QueryParams)
        {
            QueryString.Append(FString::Printf(TEXT("%s=%s&"), *Param.Key, *Param.Value));
        }
        QueryString.RemoveFromEnd(TEXT("&"));
    }
    QueryString = QueryString.Replace( TEXT(" "), TEXT("%20") );
    return QueryString;
}
