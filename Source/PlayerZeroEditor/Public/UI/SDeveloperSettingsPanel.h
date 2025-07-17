#pragma once

#include "CoreMinimal.h"
#include "Api/Blueprints/BlueprintApi.h"
#include "Widgets/SCompoundWidget.h"

struct FBlueprintListResponse;
struct FCharacterBlueprint;
class FDeveloperAccountApi;
class FRpmTextureLoader;
struct FApplication;

DECLARE_DELEGATE(FOnLogout);

class RPMNEXTGENEDITOR_API SDeveloperSettingsPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDeveloperSettingsPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void RunPanelSetup(const FString& InUserName);


	FOnLogout OnLogout;
private:
	TSharedPtr<FDeveloperAccountApi> DeveloperAccountApi;
	TSharedPtr<FBlueprintApi> BlueprintApi;
	
	TSharedPtr<STextBlock> ErrorMessageText;
	TSharedPtr<SVerticalBox> ContentBox;
	TSharedPtr<FString> SelectedComboBoxItem;
	TArray<TSharedPtr<FString>> ComboBoxItems;
	TSharedPtr<STextBlock> SelectedApplicationTextBlock;
	TArray<UTexture2D*> CharacterStyleTextures;
	TArray<TSharedPtr<FRpmTextureLoader>> ActiveLoaders;
	FString UserName;

	void UpdateErrorMessage(const FString& Message);
	void PopulateSettingsContent(TArray<FApplication> InApplicationList);
	//void OnLoadCharacterStyleClicked(const FCharacterBlueprint& Blueprint);
	void OnTextureLoaded(UTexture2D* Texture2D, TSharedPtr<SImage> Image, TSharedPtr<FRpmTextureLoader> RpmTextureLoader);
	void AddCharacterStyle(const FCharacterBlueprint& Blueprint);
	void HandleBlueprintListResponse(TSharedPtr<FBlueprintListResponse> Response, bool bWasSuccessful);
	void LoadCharacterStyleList();
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);
	void ClearLoadedCharacterModelImages();
	FReply OnLogoutClicked();
	void PopulateComboBoxItems(const TArray<FString>& Items, const FString ActiveItem);
	FText GetSelectedComboBoxItemText() const;
	FText GetWelcomeText() const;

	TArray<FApplication> ApplicationList;
};
