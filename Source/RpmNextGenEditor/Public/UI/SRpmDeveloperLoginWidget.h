// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "Widgets/SCompoundWidget.h"
#include "Containers/Map.h"

class SDeveloperSettingsPanel;
class SDeveloperLoginPanel;
class URpmDeveloperSettings;
class SEditableTextBox;

/**
 * 
 */
class RPMNEXTGENEDITOR_API SRpmDeveloperLoginWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRpmDeveloperLoginWidget)
		{
		}

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
	virtual ~SRpmDeveloperLoginWidget() override;
	
private:
	void HandleLogin(const FString& String);
	void HandleApplicationList(const FApplicationListResponse& ApplicationListResponse);
	void HandleLogout();
	TSharedPtr<SDeveloperLoginPanel> LoginPanel;
	TSharedPtr<SDeveloperSettingsPanel> SettingsPanel;

	EVisibility GetLoginPanelVisibility() const;
	EVisibility GetSettingsPanelVisibility() const;
	bool bIsLoggedIn = false;
	bool bIsInitialized = false;
	FString UserName;
	const FString DemoUserName = TEXT("Guest user");

	void SetLoggedInState(const bool IsLoggedIn);
};
