#include "SDeveloperLoginPanel.h"

#include "EditorCache.h"
#include "Auth/DevAuthTokenCache.h"
#include "Auth/DeveloperAuthApi.h"
#include "Auth/DeveloperTokenAuthStrategy.h"
#include "Auth/Models/DeveloperAuth.h"
#include "Auth/Models/DeveloperLoginRequest.h"
#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "DeveloperAccounts/Models/OrganizationListRequest.h"
#include "DeveloperAccounts/Models/OrganizationListResponse.h"
#include "Settings/RpmDeveloperSettings.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"

void SDeveloperLoginPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Sign in with your account"))
		]
        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Email:"))
		]
        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SAssignNew(EmailTextBox, SEditableTextBox)
		]
        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Password:"))
		]
        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SAssignNew(PasswordTextBox, SEditableTextBox)
			.IsPassword(true)
		]
        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SButton)
			.Text(FText::FromString("Login"))
			.OnClicked(this, &SDeveloperLoginPanel::OnLoginClicked)
		]
	];
	Initialize();
}

void SDeveloperLoginPanel::Initialize()
{
	if(!DeveloperAuthApi.IsValid())
	{
		DeveloperAuthApi = MakeShared<FDeveloperAuthApi>();
	}
	if(!DeveloperAccountApi.IsValid())
	{
		DeveloperAccountApi = MakeShared<FDeveloperAccountApi>();
	}
}

FReply SDeveloperLoginPanel::OnLoginClicked()
{
	URpmDeveloperSettings* RpmSettings = GetMutableDefault<URpmDeveloperSettings>();
	RpmSettings->Reset();
	FString Email = EmailTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();
	FEditorCache::SetString(CacheKeyEmail, Email);
	Email = Email.TrimStartAndEnd();
	Password = Password.TrimStartAndEnd();
	//Update the authentication strategy to use the developer token
	DeveloperAccountApi->SetAuthenticationStrategy(MakeShared<FDeveloperTokenAuthStrategy>());
	
	const TSharedPtr<FDeveloperLoginRequest> LoginRequest = MakeShared<FDeveloperLoginRequest>(Email, Password);
	TWeakPtr<SDeveloperLoginPanel> WeakPtrThis = StaticCastSharedRef<SDeveloperLoginPanel>(AsShared());
	DeveloperAuthApi->LoginWithEmail(LoginRequest, FOnDeveloperLoginResponse::CreateLambda([WeakPtrThis]( TSharedPtr<FDeveloperLoginResponse> Response, bool bWasSuccessful)
	{
		if(WeakPtrThis.IsValid())
		{
			WeakPtrThis.Pin()->HandleLoginResponse(Response, bWasSuccessful);
		}
	}));
	
	return FReply::Handled();
}

void SDeveloperLoginPanel::HandleLoginResponse(TSharedPtr<FDeveloperLoginResponse> Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		const FDeveloperAuth AuthData = FDeveloperAuth(Response->Data, false);
		FDevAuthTokenCache::SetAuthData(AuthData);
		OnLoginSuccess.ExecuteIfBound(Response->Data.Name);
		GetOrganizationList();
		return;
	}
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Login request failed"));
	FDevAuthTokenCache::ClearAuthData();
}

void SDeveloperLoginPanel::GetOrganizationList()
{
	TSharedPtr<FOrganizationListRequest> OrgRequest = MakeShared<FOrganizationListRequest>();
	TWeakPtr<SDeveloperLoginPanel> WeakPtrThis = StaticCastSharedRef<SDeveloperLoginPanel>(AsShared());
	DeveloperAccountApi->ListOrganizationsAsync(OrgRequest, FOnOrganizationListResponse::CreateLambda([WeakPtrThis]( TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful)
	{
		if(WeakPtrThis.IsValid())
		{
			WeakPtrThis.Pin()->HandleOrganizationListResponse(Response, bWasSuccessful);
		}
	}));
}

void SDeveloperLoginPanel::HandleOrganizationListResponse(TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (Response->Data.Num() == 0)
		{
			UE_LOG(LogReadyPlayerMe, Error, TEXT("No organizations found"));
			return;
		}
		OnOrgRequestComplete.ExecuteIfBound(Response->Data[0].Id);
		return;
	}
}
