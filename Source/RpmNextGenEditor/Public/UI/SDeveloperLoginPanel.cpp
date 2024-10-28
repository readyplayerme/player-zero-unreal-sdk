#include "SDeveloperLoginPanel.h"

#include "EditorCache.h"
#include "Auth/DevAuthTokenCache.h"
#include "Auth/DeveloperAuthApi.h"
#include "Auth/Models/DeveloperAuth.h"
#include "Auth/Models/DeveloperLoginRequest.h"
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
			.OnTextChanged(this, &SDeveloperLoginPanel::OnTextChanged)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10, 0)
		[
			SAssignNew(EmailErrorText, STextBlock)
			.Text(FText::FromString("Email is required."))
			.ColorAndOpacity(FSlateColor(FLinearColor::Red))
			.Visibility(EVisibility::Collapsed) // Initially hidden
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
			.OnTextChanged(this, &SDeveloperLoginPanel::OnTextChanged)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10, 0)
		[
			SAssignNew(PasswordErrorText, STextBlock)
			.Text(FText::FromString("Password is required."))
			.ColorAndOpacity(FSlateColor(FLinearColor::Red))
			.Visibility(EVisibility::Collapsed) // Initially hidden
		]        
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SButton)
			.Text(FText::FromString("Login"))
			.OnClicked(this, &SDeveloperLoginPanel::OnLoginClicked)
			.IsEnabled(this, &SDeveloperLoginPanel::IsInputTextValid) // Bind to enable/disable button
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10, 0)
		[
			SAssignNew(LoginErrorText, STextBlock)
			.Text(FText::FromString("Login failed. Please check your credentials."))
			.ColorAndOpacity(FSlateColor(FLinearColor::Red))
			.Visibility(EVisibility::Collapsed) // Initially hidden
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("Use Demo Account"))
			.OnClicked(this, &SDeveloperLoginPanel::OnUseDemoAccountClicked)
		]
	];
	
	Initialize();
	UpdateErrorMessages();
}

void SDeveloperLoginPanel::Initialize()
{
	EmailTextBox->SetText(FText::FromString(FEditorCache::GetString(CacheKeyEmail)));
	if(!DeveloperAuthApi.IsValid())
	{
		DeveloperAuthApi = MakeShared<FDeveloperAuthApi>();
	}
}

FReply SDeveloperLoginPanel::OnLoginClicked()
{
	bFailedLogin = false;
	UpdateErrorMessages();
	URpmDeveloperSettings* RpmSettings = GetMutableDefault<URpmDeveloperSettings>();
	RpmSettings->Reset();
	FString Email = EmailTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();
	FEditorCache::SetString(CacheKeyEmail, Email);
	Email = Email.TrimStartAndEnd();
	Password = Password.TrimStartAndEnd();
	
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

FReply SDeveloperLoginPanel::OnUseDemoAccountClicked()
{
	bFailedLogin = false;
	URpmDeveloperSettings* RpmSettings = GetMutableDefault<URpmDeveloperSettings>();
	RpmSettings->Reset();
	RpmSettings->SetupDemoAccount();
	FDeveloperAuth AuthData = FDeveloperAuth();
	AuthData.Name = TEXT("Demo User");
	AuthData.IsDemo = true;
	FDevAuthTokenCache::SetAuthData(AuthData);
	OnLoginSuccess.ExecuteIfBound(AuthData.Name);
	return FReply::Handled();
}

void SDeveloperLoginPanel::HandleLoginResponse(TSharedPtr<FDeveloperLoginResponse> Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		const FDeveloperAuth AuthData = FDeveloperAuth(Response->Data, false);
		FDevAuthTokenCache::SetAuthData(AuthData);
		OnLoginSuccess.ExecuteIfBound(Response->Data.Name);
		return;
	}
	bFailedLogin = true;
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Login request failed"));
	FDevAuthTokenCache::ClearAuthData();
	UpdateErrorMessages();
}

void SDeveloperLoginPanel::OnTextChanged(const FText& Text)
{
	UpdateErrorMessages();
}

void SDeveloperLoginPanel::UpdateErrorMessages()
{
	EmailErrorText->SetVisibility(EmailTextBox->GetText().IsEmpty() ? EVisibility::Visible : EVisibility::Collapsed);
	PasswordErrorText->SetVisibility(PasswordTextBox->GetText().IsEmpty() ? EVisibility::Visible : EVisibility::Collapsed);
	LoginErrorText->SetVisibility(bFailedLogin ? EVisibility::Visible : EVisibility::Collapsed);
}

bool SDeveloperLoginPanel::IsInputTextValid() const
{
	return !EmailTextBox->GetText().IsEmpty() && !PasswordTextBox->GetText().IsEmpty();
}
