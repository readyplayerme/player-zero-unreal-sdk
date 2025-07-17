#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "DeveloperAccounts/Models/ApplicationListRequest.h"
#include "DeveloperAccounts/Models/ApplicationListResponse.h"
#include "DeveloperAccounts/Models/OrganizationListRequest.h"
#include "DeveloperAccounts/Models/OrganizationListResponse.h"
<<<<<<< HEAD:Source/PlayerZeroEditor/Private/DeveloperAccounts/DeveloperAccountApi.cpp
#include "Interfaces/IHttpResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"
=======
#include "Settings/RpmDeveloperSettings.h"
>>>>>>> origin/develop:Source/RpmNextGenEditor/Private/DeveloperAccounts/DeveloperAccountApi.cpp

FDeveloperAccountApi::FDeveloperAccountApi()
{    
}

FDeveloperAccountApi::FDeveloperAccountApi(const TSharedPtr<IAuthenticationStrategy>& InAuthenticationStrategy) : FWebApiWithAuth(InAuthenticationStrategy)
{
    if (UPlayerZeroDeveloperSettings* Settings = GetMutableDefault<UPlayerZeroDeveloperSettings>())
    {
        ApiBaseUrl = Settings->GetApiBaseUrl();
    }
}

void FDeveloperAccountApi::ListApplicationsAsync(TSharedPtr<FApplicationListRequest> Request, FOnApplicationListResponse OnComplete)
{
    // TODO find better way to get settings (or move to editor only code)
<<<<<<< HEAD:Source/PlayerZeroEditor/Private/DeveloperAccounts/DeveloperAccountApi.cpp
    const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
    ApiBaseUrl = PlayerZeroSettings->GetApiBaseUrl();
    const FString QueryString = BuildQueryString(Request.Params);
=======
    const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
    ApiBaseUrl = RpmSettings->GetApiBaseUrl();
    const FString QueryString = BuildQueryString(Request->Params);
>>>>>>> origin/develop:Source/RpmNextGenEditor/Private/DeveloperAccounts/DeveloperAccountApi.cpp
    const FString Url = FString::Printf(TEXT("%s/v1/applications%s"), *ApiBaseUrl, *QueryString);
    TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
    ApiRequest->Url = Url;
    SendRequestWithAuth<FApplicationListResponse>(ApiRequest, [OnComplete](TSharedPtr<FApplicationListResponse> Response, bool bWasSuccessful, int32 StatusCode)
    {
        //UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed "));
        OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
    });
}

void FDeveloperAccountApi::ListOrganizationsAsync(TSharedPtr<FOrganizationListRequest> Request, FOnOrganizationListResponse OnComplete)
{
    // TODO find better way to get settings (or move to editor only code)
<<<<<<< HEAD:Source/PlayerZeroEditor/Private/DeveloperAccounts/DeveloperAccountApi.cpp
    const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
    ApiBaseUrl = PlayerZeroSettings->GetApiBaseUrl();
    const FString QueryString = BuildQueryString(Request.Params);
=======
    const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
    ApiBaseUrl = RpmSettings->GetApiBaseUrl();
    const FString QueryString = BuildQueryString(Request->Params);
>>>>>>> origin/develop:Source/RpmNextGenEditor/Private/DeveloperAccounts/DeveloperAccountApi.cpp
    const FString Url = FString::Printf(TEXT("%s/v1/organizations%s"), *ApiBaseUrl, *QueryString);
    TSharedPtr<FApiRequest> ApiRequest = MakeShared<FApiRequest>();
    ApiRequest->Url = Url;
    SendRequestWithAuth<FOrganizationListResponse>(ApiRequest, [OnComplete](TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful, int32 StatusCode)
    {
        //UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed "));
        OnComplete.ExecuteIfBound(Response, bWasSuccessful && Response.IsValid());
    });
}


FString FDeveloperAccountApi::BuildQueryString(const TMap<FString, FString>& Params)
{
    if (Params.Num() == 0) return FString();
    FString QueryString = TEXT("?");
    for (const auto& Param : Params)
    {
        QueryString += Param.Key + TEXT("=") + Param.Value + TEXT("&");
    }
    QueryString.RemoveFromEnd(TEXT("&"));
    return QueryString;
}
