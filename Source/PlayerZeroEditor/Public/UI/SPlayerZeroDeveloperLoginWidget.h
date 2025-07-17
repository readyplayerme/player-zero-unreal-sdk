// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Auth/DeveloperAuthApi.h"
#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "DeveloperAccounts/Models/ApplicationListResponse.h"
#include "Widgets/SCompoundWidget.h"
#include "Containers/Map.h"
#include "PlayerZero/Public/Api/Assets/Models/Asset.h"
#include "Api/Blueprints/Models/CharacterBlueprint.h"

class FBlueprintApi;
struct FBlueprintListResponse;
class FPlayerZeroTextureLoader;
struct FDeveloperLoginResponse;
class UPlayerZeroDeveloperSettings;
class UDeveloperAuthApi;
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
	virtual ~SPlayerZeroDeveloperLoginWidget() override;

private:
	TSharedPtr<SVerticalBox> ContentBox;
	TSharedPtr<SEditableTextBox> EmailTextBox;
	TSharedPtr<SEditableTextBox> PasswordTextBox;
	TSharedPtr<STextBlock> SelectedApplicationTextBlock;
	TSharedPtr<FString> SelectedComboBoxItem;
	TArray<TSharedPtr<FString>> ComboBoxItems;
	TArray<UTexture2D*> CharacterStyleTextures;

	EVisibility GetLoginViewVisibility() const;
	EVisibility GetLoggedInViewVisibility() const;
	TArray<TSharedPtr<FPlayerZeroTextureLoader>> ActiveLoaders;
	TSharedPtr<FDeveloperAccountApi> DeveloperAccountApi;
	TSharedPtr<FDeveloperAuthApi> DeveloperAuthApi;
	TSharedPtr<FBlueprintApi> BlueprintApi;
	static constexpr const TCHAR* CacheKeyEmail = TEXT("Email");
	bool bIsLoggedIn = false;
	bool bIsInitialized = false;
	FString UserName;
	TArray<FApplication> UserApplications;
	FText GetWelcomeText() const;
	const FString DemoUserName = TEXT("Guest user");
	FText GetSelectedComboBoxItemText() const;

	FReply OnLoginClicked();
	FReply OnLogoutClicked();
	TMap<FString, FCharacterBlueprint> LoadedBlueprints;
	void Initialize();
	void GetOrgList();
	void ClearLoadedCharacterModelImages();
	void LoadBlueprintList();
	void HandleLoginResponse(const FDeveloperLoginResponse& Response, bool bWasSuccessful);
	void HandleOrganizationListResponse(const FOrganizationListResponse& Response, bool bWasSuccessful);
	void HandleApplicationListResponse(const FApplicationListResponse& Response, bool bWasSuccessful);
	void HandleBlueprintListResponse(const FBlueprintListResponse& Response, bool bWasSuccessful);
	void OnLoadBlueprintClicked(const FCharacterBlueprint& CharacterBlueprint);
	void SetLoggedInState(const bool IsLoggedIn);
	void PopulateComboBoxItems(const TArray<FString>& Items, const FString ActiveItem);
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);
	UFUNCTION()
	void OnTextureLoaded(UTexture2D* Texture2D, TSharedPtr<SImage> SImage, TSharedPtr<FPlayerZeroTextureLoader> LoaderToRemove);
	void AddCharacterBlueprint(const FCharacterBlueprint& CharacterBlueprint);
};
