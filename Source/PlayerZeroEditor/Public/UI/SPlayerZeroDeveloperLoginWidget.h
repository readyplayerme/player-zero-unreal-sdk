// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "Widgets/SCompoundWidget.h"
#include "Containers/Map.h"

<<<<<<< HEAD:Source/PlayerZeroEditor/Public/UI/SPlayerZeroDeveloperLoginWidget.h
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
	TSharedPtr<SDeveloperLoginPanel> LoginPanel;
	TSharedPtr<SDeveloperSettingsPanel> SettingsPanel;

<<<<<<< HEAD:Source/PlayerZeroEditor/Public/UI/SPlayerZeroDeveloperLoginWidget.h
	EVisibility GetLoginViewVisibility() const;
	EVisibility GetLoggedInViewVisibility() const;
	TArray<TSharedPtr<FPlayerZeroTextureLoader>> ActiveLoaders;
	TSharedPtr<FEditorAssetLoader> AssetLoader;
	TSharedPtr<FAssetApi> AssetApi;
	TSharedPtr<FDeveloperAccountApi> DeveloperAccountApi;
	TSharedPtr<FDeveloperAuthApi> DeveloperAuthApi;
	static constexpr const TCHAR* CacheKeyEmail = TEXT("Email");
=======
	EVisibility GetLoginPanelVisibility() const;
	EVisibility GetSettingsPanelVisibility() const;
>>>>>>> origin/develop:Source/RpmNextGenEditor/Public/UI/SRpmDeveloperLoginWidget.h
	bool bIsLoggedIn = false;
	bool bIsInitialized = false;
	FString UserName;
	const FString DemoUserName = TEXT("Guest user");

	void SetLoggedInState(const bool IsLoggedIn);
<<<<<<< HEAD:Source/PlayerZeroEditor/Public/UI/SPlayerZeroDeveloperLoginWidget.h
	void PopulateComboBoxItems(const TArray<FString>& Items, const FString ActiveItem);
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);
	UFUNCTION()
	void OnTextureLoaded(UTexture2D* Texture2D, TSharedPtr<SImage> SImage, TSharedPtr<FPlayerZeroTextureLoader> LoaderToRemove);
	void AddCharacterStyle(const FAsset& StyleAsset);
=======
>>>>>>> origin/develop:Source/RpmNextGenEditor/Public/UI/SRpmDeveloperLoginWidget.h
};
