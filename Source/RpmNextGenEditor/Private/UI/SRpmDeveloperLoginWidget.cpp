// Copyright Ready Player Me

#include "UI/SRpmDeveloperLoginWidget.h"
#include "Auth/DevAuthTokenCache.h"
#include "RpmNextGen.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Auth/Models/DeveloperAuth.h"
#include "UI/SDeveloperLoginPanel.h"
#include "UI/SDeveloperSettingsPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRpmDeveloperLoginWidget::Construct(const FArguments& InArgs)
{
	FDeveloperAuth AuthData = FDevAuthTokenCache::GetAuthData();

	bIsLoggedIn = false;
	if(AuthData.IsValid())
	{
		FDevAuthTokenCache::SetAuthData(AuthData);
		bIsLoggedIn = AuthData.IsValid();
		UserName = AuthData.Name;
	}
	else
	{
		UserName = "User";
		FDevAuthTokenCache::ClearAuthData();
	}
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SAssignNew(LoginPanel, SDeveloperLoginPanel)
			.Visibility(this, &SRpmDeveloperLoginWidget::GetLoginPanelVisibility)
		]
		+ SVerticalBox::Slot()
		[
			SAssignNew(SettingsPanel, SDeveloperSettingsPanel)
			.Visibility(this, &SRpmDeveloperLoginWidget::GetSettingsPanelVisibility)
		]
	];

	if(LoginPanel)
	{
		LoginPanel->OnLoginSuccess.BindRaw( this, &SRpmDeveloperLoginWidget::HandleLogin );
	}

	if(SettingsPanel)
	{
		SettingsPanel->OnLogout.BindRaw( this, &SRpmDeveloperLoginWidget::HandleLogout );
		if(bIsLoggedIn)
		{
			SettingsPanel->RunPanelSetup(UserName);
		}
	}

	SetLoggedInState(bIsLoggedIn);
}

SRpmDeveloperLoginWidget::~SRpmDeveloperLoginWidget()
{
}

void SRpmDeveloperLoginWidget::HandleLogin(const FString& String)
{
	UE_LOG(LogReadyPlayerMe, Log, TEXT("Login success: %s"), *String);
	if(SettingsPanel)
	{
		SettingsPanel->RunPanelSetup(String);
	}
	SetLoggedInState(true);
}

void SRpmDeveloperLoginWidget::HandleLogout()
{
	SetLoggedInState(false);
}

void SRpmDeveloperLoginWidget::SetLoggedInState(bool IsLoggedIn)
{
	bIsLoggedIn = IsLoggedIn;
	Invalidate(EInvalidateWidget::Layout); 
}

EVisibility SRpmDeveloperLoginWidget::GetLoginPanelVisibility() const
{
	return bIsLoggedIn ? EVisibility::Collapsed : EVisibility::Visible;
}

EVisibility SRpmDeveloperLoginWidget::GetSettingsPanelVisibility() const
{
	return bIsLoggedIn ? EVisibility::Visible : EVisibility::Collapsed;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
