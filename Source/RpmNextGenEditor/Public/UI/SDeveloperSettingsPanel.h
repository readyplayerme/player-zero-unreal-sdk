#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FDeveloperAccountApi;
struct FOrganizationListResponse;
struct FApplicationListResponse;
class FEditorAssetLoader;
class FRpmTextureLoader;
struct FAsset;
class FAssetApi;
struct FAssetListResponse;
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
	TSharedPtr<FAssetApi> AssetApi;
	
	TSharedPtr<SVerticalBox> ContentBox;
	TSharedPtr<FString> SelectedComboBoxItem;
	TArray<TSharedPtr<FString>> ComboBoxItems;
	TSharedPtr<STextBlock> SelectedApplicationTextBlock;
	TArray<UTexture2D*> CharacterStyleTextures;
	TMap<FString, FAsset> CharacterStyleAssets;
	TArray<TSharedPtr<FRpmTextureLoader>> ActiveLoaders;
	TSharedPtr<FEditorAssetLoader> AssetLoader;
	FString UserName;

	void PopulateSettingsContent(TArray<FApplication> InApplicationList);
	void OnLoadBaseModelClicked(const FAsset& Asset);
	void OnTextureLoaded(UTexture2D* Texture2D, TSharedPtr<SImage> Image, TSharedPtr<FRpmTextureLoader> RpmTextureLoader);
	void AddCharacterStyle(const FAsset& Asset);
	void HandleBaseModelListResponse(TSharedPtr<FAssetListResponse> Response, bool bWasSuccessful);
	void HandleApplicationListResponse(TSharedPtr<FApplicationListResponse> Response, bool bWasSuccessful);
	void HandleOrganizationListResponse(TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful);
	void LoadBaseModelList();
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);
	void ClearLoadedCharacterModelImages();
	FReply OnLogoutClicked();
	void PopulateComboBoxItems(const TArray<FString>& Items, const FString ActiveItem);
	FText GetSelectedComboBoxItemText() const;
	FText GetWelcomeText() const;

	TArray<FApplication> ApplicationList;
};
