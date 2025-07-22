#include "Api/Common/WebApi.h"
#include "HttpModule.h"
#include "PlayerZero.h"
#include "Interfaces/IHttpResponse.h"

FWebApi::FWebApi()
{
    Http = &FHttpModule::Get();
}

FWebApi::~FWebApi()
{
    
}

void FWebApi::DispatchRaw(TSharedPtr<FApiRequest> ApiRequest, FOnDispatchComplete OnComplete)
{
    TSharedPtr<IHttpRequest> Request = Http->CreateRequest();
    FString Url = ApiRequest->Url + BuildQueryString(ApiRequest->QueryParams);
    Request->SetURL(Url);
    Request->SetVerb(ApiRequest->GetVerb());
    Request->SetTimeout(10);
    FString Headers;
    for (const auto& Header : ApiRequest->Headers)
    {
        Request->SetHeader(Header.Key, Header.Value);
        Headers.Append(FString::Printf(TEXT("%s: %s\n"), *Header.Key, *Header.Value));
    }

    if (!ApiRequest->Payload.IsEmpty() && ApiRequest->Method != ERequestMethod::GET)
    {
        Request->SetContentAsString(ApiRequest->Payload);
    }
    Request->OnProcessRequestComplete().BindLambda(
        [ApiRequest, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
        {
            if (bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
            {
                ApiRequest->OnapiRequestComplete.ExecuteIfBound(ApiRequest, Response, true);
                return;
            }
            FString ErrorMessage = Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed");
            UE_LOG(LogPlayerZero, Warning, TEXT("WebApi from URL %s request failed: %s"), *Request->GetURL(), *ErrorMessage);
            ApiRequest->OnRequestComplete.ExecuteIfBound(ApiRequest, Response, false);
            OnComplete.ExecuteIfBound(ApiRequest, Response, bSuccess);
        }
    );
    Request->ProcessRequest();
}

void FWebApi::OnProcessResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TSharedPtr<FApiRequest> ApiRequest)
{
    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        ApiRequest->OnRequestComplete.ExecuteIfBound(ApiRequest, Response, true);
        return;
    }
    FString ErrorMessage = Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed");
    UE_LOG(LogPlayerZero, Warning, TEXT("WebApi from URL %s request failed: %s"), *Request->GetURL(), *ErrorMessage);
    ApiRequest->OnRequestComplete.ExecuteIfBound(ApiRequest, Response, false);
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
