// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Containers/Map.h"

class SPlayerZeroSettingsPanel;
class SPlayerZeroLoginPanel;
class FBlueprintApi;
struct FBlueprintListResponse;
class FPlayerZeroTextureLoader;
struct FDeveloperLoginResponse;
class UPlayerZeroDeveloperSettings;
class UDeveloperAuthApi;
=======
class SDeveloperSettingsPanel;
class SDeveloperLoginPanel;
class URpmDeveloperSettings;
>>>>>>> origin/develop:Source/RpmNextGenEditor/Public/UI/SRpmDeveloperLoginWidget.h
class SEditableTextBox;

/**
 * 
 */
class PLAYERZEROEDITOR_API SPlayerZeroDeveloperLoginWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerZeroDeveloperLoginWidget)
	{
	}

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
<<<<<<< HEAD:Source/PlayerZeroEditor/Public/UI/SPlayerZeroDeveloperLoginWidget.h
	virtual ~SPlayerZeroDeveloperLoginWidget() override;

=======
	virtual ~SRpmDeveloperLoginWidget() override;
	
>>>>>>> origin/develop:Source/RpmNextGenEditor/Public/UI/SRpmDeveloperLoginWidget.h
private:
	void HandleLogin(const FString& String);
	void HandleLogout();
	TSharedPtr<SPlayerZeroLoginPanel> LoginPanel;
	TSharedPtr<SPlayerZeroSettingsPanel> SettingsPanel;

	EVisibility GetLoginPanelVisibility() const;
	EVisibility GetSettingsPanelVisibility() const;
	bool bIsLoggedIn = false;
	bool bIsInitialized = false;
	FString UserName;
	const FString DemoUserName = TEXT("Guest user");

	void SetLoggedInState(const bool IsLoggedIn);
};
