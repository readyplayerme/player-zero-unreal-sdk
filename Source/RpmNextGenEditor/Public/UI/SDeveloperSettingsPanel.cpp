#include "SDeveloperSettingsPanel.h"
#include "EditorAssetLoader.h"
#include "RpmNextGen.h"
#include "RpmTextureLoader.h"
#include "SDeveloperLoginPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Api/Assets/AssetApi.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Assets/Models/AssetListRequest.h"
#include "Api/Assets/Models/AssetListResponse.h"
#include "Auth/DevAuthTokenCache.h"
#include "Auth/DeveloperTokenAuthStrategy.h"
#include "Auth/Models/DeveloperAuth.h"
#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "DeveloperAccounts/Models/ApplicationListRequest.h"
#include "DeveloperAccounts/Models/ApplicationListResponse.h"
#include "DeveloperAccounts/Models/OrganizationListRequest.h"
#include "DeveloperAccounts/Models/OrganizationListResponse.h"
#include "Settings/RpmDeveloperSettings.h"
#include "Utilities/RpmImageHelper.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Images/SImage.h"

void SDeveloperSettingsPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0)
			[
				SNew(STextBlock)
				.Text(this, &SDeveloperSettingsPanel::GetWelcomeText)
				
			]
			+ SHorizontalBox::Slot()
			  .AutoWidth()
			  .HAlign(HAlign_Right)
			[
				SNew(SButton)
				.Text(FText::FromString("Logout"))
				.OnClicked(this, &SDeveloperSettingsPanel::OnLogoutClicked)
			]
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Project Settings"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Select the Ready Player Me application to link to project"))
		]

		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&ComboBoxItems)
			.OnSelectionChanged(this, &SDeveloperSettingsPanel::OnComboBoxSelectionChanged)
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
				{
				return SNew(STextBlock).Text(FText::FromString(*Item));
				})
			[
				SAssignNew(SelectedApplicationTextBlock, STextBlock).Text(
					this, &SDeveloperSettingsPanel::GetSelectedComboBoxItemText)
			]
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Character Models"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Here you can import your character models from Studio"))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SAssignNew(ErrorMessageText, STextBlock)
			.ColorAndOpacity(FSlateColor(FLinearColor::Red))
			.Visibility(EVisibility::Collapsed) // Hidden initially
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .FillHeight(1.0f) // Allows the scroll box to take up remaining space
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(ContentBox, SVerticalBox)
			]
		]	
	];

	UpdateErrorMessage("");
}

void SDeveloperSettingsPanel::RunPanelSetup(const FString& InUserName)
{
	InUserName.IsEmpty() ? UserName = TEXT("User") : UserName = InUserName;
	UpdateErrorMessage("");
	const FDeveloperAuth DevAuthData = FDevAuthTokenCache::GetAuthData();
	if (!AssetApi.IsValid())
	{
		CharacterStyleAssets = TMap<FString, FRpmAsset>();
		ActiveLoaders = TArray<TSharedPtr<FRpmTextureLoader>>();
		AssetApi = MakeShared<FAssetApi>();
	}
	else
	{
		AssetApi->CancelAllRequests();
	}
	if (!DevAuthData.IsDemo)
	{
		AssetApi->SetAuthenticationStrategy(MakeShared<FDeveloperTokenAuthStrategy>());
	}
	else
	{
		AssetApi->SetAuthenticationStrategy(nullptr);
	}

	if(!DeveloperAccountApi.IsValid())
	{
		DeveloperAccountApi = MakeShared<FDeveloperAccountApi>();

	}
	else
	{
		AssetApi->CancelAllRequests();
	}
	if(!DevAuthData.IsDemo)
	{
		DeveloperAccountApi->SetAuthenticationStrategy(MakeShared<FDeveloperTokenAuthStrategy>());
	}
	else
	{
		DeveloperAccountApi->SetAuthenticationStrategy(nullptr);
	}

	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	if(RpmSettings->ApiKey.IsEmpty() && RpmSettings->ApiProxyUrl.IsEmpty())
	{
		UpdateErrorMessage("API Key and Proxy URL is not set. Please check your Ready Player Me Settings.");
		return;
	}
	
	const TSharedPtr<FOrganizationListRequest> OrgRequest = MakeShared<FOrganizationListRequest>();
	TWeakPtr<SDeveloperSettingsPanel> WeakPtrThis = StaticCastSharedRef<SDeveloperSettingsPanel>(AsShared());
	DeveloperAccountApi->ListOrganizationsAsync(OrgRequest, FOnOrganizationListResponse::CreateLambda([WeakPtrThis]( TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful)
	{
		if(WeakPtrThis.IsValid())
		{
			WeakPtrThis.Pin()->HandleOrganizationListResponse(Response, bWasSuccessful);
		}
	}));
}

void SDeveloperSettingsPanel::UpdateErrorMessage(const FString& Message)
{
	if (Message.IsEmpty())
	{
		ErrorMessageText->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		ErrorMessageText->SetText(FText::FromString(Message));
		ErrorMessageText->SetVisibility(EVisibility::Visible);
	}
}

void SDeveloperSettingsPanel::PopulateSettingsContent(TArray<FApplication> InApplicationList)
{
	if(InApplicationList.Num() == 0)
	{
		UE_LOG( LogReadyPlayerMe, Error, TEXT("No applications found, check your Ready PLayer Me Settings") );
		return;
	}
	const FDeveloperAuth DevAuthData = FDevAuthTokenCache::GetAuthData();
	if (!AssetApi.IsValid())
	{
		CharacterStyleAssets = TMap<FString, FRpmAsset>();
		ActiveLoaders = TArray<TSharedPtr<FRpmTextureLoader>>();
		AssetApi = MakeShared<FAssetApi>();
		if (!DevAuthData.IsDemo)
		{
			AssetApi->SetAuthenticationStrategy(MakeShared<FDeveloperTokenAuthStrategy>());
		}
	}
	else
	{
		AssetApi->CancelAllRequests();
		if (!DevAuthData.IsDemo)
		{
			AssetApi->SetAuthenticationStrategy(MakeShared<FDeveloperTokenAuthStrategy>());
		}
		else
		{
			AssetApi->SetAuthenticationStrategy(nullptr);
		}
	}
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	ApplicationList = InApplicationList;
	FString Active;
	TArray<FString> Items;
	for (const FApplication& App : ApplicationList)
	{
		Items.Add(App.Name);
		if (App.Id == RpmSettings->ApplicationId)
		{
			Active = App.Name;
		}
	}
	if (Active.IsEmpty() && Items.Num() > 0)
	{
		const auto NewActiveItem = MakeShared<FString>(Items[0]);
		OnComboBoxSelectionChanged(NewActiveItem, ESelectInfo::Direct);
		SelectedApplicationTextBlock->SetText(FText::FromString(*NewActiveItem));
	}
	PopulateComboBoxItems(Items, Active);
	LoadCharacterStyleList();
}

void SDeveloperSettingsPanel::LoadCharacterStyleList()
{
	const URpmDeveloperSettings* RpmSettings = GetDefault<URpmDeveloperSettings>();
	if (RpmSettings->ApplicationId.IsEmpty())
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Application ID is empty, unable to load base models."));
		return;
	}
	TSharedPtr<FAssetListRequest> Request = MakeShared<FAssetListRequest>();
	FAssetListQueryParams Params = FAssetListQueryParams();
	Params.ApplicationId = RpmSettings->ApplicationId;
	Params.Type = FAssetApi::CharacterStyleAssetType;
	Request->Params = Params;
	TWeakPtr<SDeveloperSettingsPanel> WeakPtrThis = StaticCastSharedRef<SDeveloperSettingsPanel>(AsShared());
	AssetApi->ListAssetsAsync(Request, FOnListAssetsResponse::CreateLambda( [WeakPtrThis](TSharedPtr<FAssetListResponse> Response, bool bWasSuccessful)
	{
		if(WeakPtrThis != nullptr && WeakPtrThis.IsValid())
		{
			WeakPtrThis.Pin()->HandleCharacterStyleListResponse(Response, bWasSuccessful);
		}
	}));
}

void SDeveloperSettingsPanel::AddCharacterStyle(const FRpmAsset& Asset)
{
	TSharedPtr<SImage> ImageWidget;
	const FVector2D ImageSize(100.0f, 100.0f);

	ContentBox->AddSlot()
			  .AutoHeight()
			  .Padding(5)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(5)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			  .AutoHeight()
			  .HAlign(HAlign_Left)
			[
				SAssignNew(ImageWidget, SImage).
				DesiredSizeOverride(ImageSize)
			]
			+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(5, 5)
			[
				SNew(SBox)
				.WidthOverride(100.0f)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Text(FText::FromString("Import"))
					.OnClicked_Lambda([this, Asset]() -> FReply
					{
						OnLoadCharacterStyleClicked(Asset);
						return FReply::Handled();
					})
				]

			]
		]
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .VAlign(VAlign_Top)
		  .Padding(10, 10, 0, 0)
		[
			SNew(SEditableText)
			   .Text(FText::FromString(FString::Printf(TEXT("ID: %s"), *Asset.Id)))
			   .IsReadOnly(true)
			   .IsCaretMovedWhenGainFocus(false)
			   .SelectAllTextWhenFocused(false)
			   .MinDesiredWidth(100.0f)
		]
	];

	TSharedPtr<FRpmTextureLoader> ImageLoader = MakeShared<FRpmTextureLoader>();
	ActiveLoaders.Add(ImageLoader);
	ImageLoader->OnTextureLoaded.BindRaw(this, &SDeveloperSettingsPanel::OnTextureLoaded, ImageWidget, ImageLoader);
	ImageLoader->LoadIconFromAsset(Asset);
}

void SDeveloperSettingsPanel::OnLoadCharacterStyleClicked(const FRpmAsset& Asset)
{
	AssetLoader = MakeShared<FEditorAssetLoader>();
	AssetLoader->LoadBCharacterStyleAsset(Asset);
}

void SDeveloperSettingsPanel::OnTextureLoaded(UTexture2D* Texture2D, TSharedPtr<SImage> Image, TSharedPtr<FRpmTextureLoader> RpmTextureLoader)
{
	if (Texture2D)
	{
		Texture2D->AddToRoot();
		CharacterStyleTextures.Add(Texture2D);
		FRpmImageHelper::LoadTextureToSImage(Texture2D, FVector2D(100.0f, 100.0f), Image);
	}
	ActiveLoaders.Remove(RpmTextureLoader);
}

void SDeveloperSettingsPanel::HandleCharacterStyleListResponse(TSharedPtr<FAssetListResponse> Response, bool bWasSuccessful)
{
	CharacterStyleAssets.Empty();
	if(bWasSuccessful && Response.IsValid())
	{
		if(Response->Data.Num() == 0)
		{
			UE_LOG(LogReadyPlayerMe, Error, TEXT("No Avatar styles found. Make sure you have uploaded your character models to Ready Player Me Studio"));
			UpdateErrorMessage("No Avatar styles found. Make sure you have uploaded your character models to Ready Player Me Studio");
			return;
		}
		for (FRpmAsset Asset : Response->Data)
		{
			CharacterStyleAssets.Add(Asset.Id, Asset);
			AddCharacterStyle(Asset);
		}
		UpdateErrorMessage("");
		return;
	}
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to list base models"));
	UpdateErrorMessage("Failed to load base models. Please try again.");
}

void SDeveloperSettingsPanel::HandleOrganizationListResponse(TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (Response->Data.Num() == 0)
		{
			UE_LOG(LogReadyPlayerMe, Error, TEXT("No organizations found"));
			return;
		}
		TSharedPtr<FApplicationListRequest> Request = MakeShared<FApplicationListRequest>();
		Request->Params.Add("organizationId", Response->Data[0].Id);
		TWeakPtr<SDeveloperSettingsPanel> WeakPtrThis = StaticCastSharedRef<SDeveloperSettingsPanel>(AsShared());
		DeveloperAccountApi->ListApplicationsAsync(Request, FOnApplicationListResponse::CreateLambda( [WeakPtrThis](TSharedPtr<FApplicationListResponse> Response, bool bWasSuccessful)
		{
			if(WeakPtrThis.IsValid())
			{
				WeakPtrThis.Pin()->HandleApplicationListResponse(Response, bWasSuccessful);
			}
		}));
		UpdateErrorMessage("");
		return;
	}
	UpdateErrorMessage("Failed to fetch organizations. Please check your network connection.");
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to fetch organizations"))
}

void SDeveloperSettingsPanel::HandleApplicationListResponse(TSharedPtr<FApplicationListResponse> Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response.IsValid())
	{
		PopulateSettingsContent(Response->Data);
		UpdateErrorMessage("");
		return;
	}
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to fetch applications"));
}

void SDeveloperSettingsPanel::PopulateComboBoxItems(const TArray<FString>& Items, const FString ActiveItem)
{
	ComboBoxItems.Empty();
	for (const FString& Item : Items)
	{
		ComboBoxItems.Add(MakeShared<FString>(Item));
	}
	SelectedComboBoxItem = MakeShared<FString>(ActiveItem);
}

FText SDeveloperSettingsPanel::GetSelectedComboBoxItemText() const
{
	return SelectedComboBoxItem.IsValid() && !SelectedComboBoxItem->IsEmpty()
			   ? FText::FromString(*SelectedComboBoxItem)
			   : FText::FromString("Select an option");
}

FText SDeveloperSettingsPanel::GetWelcomeText() const
{
	return FText::Format(FText::FromString("Welcome {0}"), FText::FromString(UserName));
}

void SDeveloperSettingsPanel::OnComboBoxSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
{
	SelectedComboBoxItem = NewValue;
	FApplication* Application = ApplicationList.FindByPredicate([&](const FApplication& Item)
	{
		return Item.Name == *NewValue;
	});
	if (Application)
	{
		URpmDeveloperSettings* RpmSettings = GetMutableDefault<URpmDeveloperSettings>();
		RpmSettings->ApplicationId = Application->Id;
		RpmSettings->SaveConfig();
	}
}

FReply SDeveloperSettingsPanel::OnLogoutClicked()
{
	URpmDeveloperSettings* RpmSettings = GetMutableDefault<URpmDeveloperSettings>();
	RpmSettings->Reset();

	if (ContentBox.IsValid())
	{
		ContentBox->ClearChildren();
	}
	ComboBoxItems.Empty();

	ClearLoadedCharacterModelImages();
	FDevAuthTokenCache::ClearAuthData();
	if(AssetApi.IsValid())
	{
		AssetApi->CancelAllRequests();
	}

	OnLogout.ExecuteIfBound();
	return FReply::Handled();
}

void SDeveloperSettingsPanel::ClearLoadedCharacterModelImages()
{
	for (const auto Texture : CharacterStyleTextures)
	{
		Texture->RemoveFromRoot();
	}
	CharacterStyleTextures.Empty();
}
