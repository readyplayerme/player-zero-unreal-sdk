// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SPlayerZeroDeveloperLoginWidget.h"
#include "Auth/DevAuthTokenCache.h"
#include "EditorCache.h"
#include "PlayerZero.h"
#include "SlateOptMacros.h"
#include "DeveloperAccounts/DeveloperAccountApi.h"
#include "Auth/DeveloperTokenAuthStrategy.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "PlayerZeroTextureLoader.h"
#include "Auth/DeveloperAuthApi.h"
#include "Auth/Models/DeveloperAuth.h"
#include "Auth/Models/DeveloperLoginRequest.h"
#include "DeveloperAccounts/Models/ApplicationListRequest.h"
#include "DeveloperAccounts/Models/ApplicationListResponse.h"
#include "DeveloperAccounts/Models/OrganizationListRequest.h"
#include "DeveloperAccounts/Models/OrganizationListResponse.h"
#include "Settings/PlayerZeroDeveloperSettings.h"
#include "Utilities/PlayerZeroImageHelper.h"
#include "Widgets/Layout/SScrollBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlayerZeroDeveloperLoginWidget::Construct(const FArguments& InArgs)
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
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Sign in with your Ready Player Me Studio account"))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Email:"))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SAssignNew(EmailTextBox, SEditableTextBox)
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Password:"))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SAssignNew(PasswordTextBox, SEditableTextBox)
			.IsPassword(true)
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("Login"))
			.OnClicked(this, &SPlayerZeroDeveloperLoginWidget::OnLoginClicked)
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("Use Demo Account"))
			.OnClicked(this, &SPlayerZeroDeveloperLoginWidget::OnUseDemoAccountClicked)
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0)
			[
				SNew(STextBlock)
				.Text(this, &SPlayerZeroDeveloperLoginWidget::GetWelcomeText)
				.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
			]
			+ SHorizontalBox::Slot()
			  .AutoWidth()
			  .HAlign(HAlign_Right)
			[
				SNew(SButton)
				.Text(FText::FromString("Logout"))
				.OnClicked(this, &SPlayerZeroDeveloperLoginWidget::OnLogoutClicked)
				.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
			]
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Project Settings"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Select the Ready Player Me application to link to project"))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
		]

		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&ComboBoxItems)
			.OnSelectionChanged(this, &SPlayerZeroDeveloperLoginWidget::OnComboBoxSelectionChanged)
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
			                                    {
				                                    return SNew(STextBlock).Text(FText::FromString(*Item));
			                                    })
			[
				SAssignNew(SelectedApplicationTextBlock, STextBlock).Text(
					this, &SPlayerZeroDeveloperLoginWidget::GetSelectedComboBoxItemText)
			]
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Character Models"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Here you can import your character models from Studio"))
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
		]
		+ SVerticalBox::Slot()
		  .Padding(10)
		  .FillHeight(1.0f) // Allows the scroll box to take up remaining space
		[
			SNew(SScrollBox)
			.Visibility(this, &SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility)
			+ SScrollBox::Slot()
			[
				SAssignNew(ContentBox, SVerticalBox)
			]
		]	
	];

	EmailTextBox->SetText(FText::FromString(FEditorCache::GetString(CacheKeyEmail)));
	Initialize();
}

void SPlayerZeroDeveloperLoginWidget::Initialize()
{
	if (bIsInitialized)
	{
		return;
	}

	ActiveLoaders = TArray<TSharedPtr<FPlayerZeroTextureLoader>>();
	const FDeveloperAuth DevAuthData = FDevAuthTokenCache::GetAuthData();
	if (!DeveloperAuthApi.IsValid())
	{
		DeveloperAuthApi = MakeShared<FDeveloperAuthApi>();

		DeveloperAuthApi->OnLoginResponse.BindRaw(this, &SPlayerZeroDeveloperLoginWidget::HandleLoginResponse);
	}

	// TODO update
	// if (!AssetApi.IsValid())
	// {
	// 	AssetApi = MakeShared<FAssetApi>();
	// 	if (!DevAuthData.IsDemo)
	// 	{
	// 		AssetApi->SetAuthenticationStrategy(MakeShared<DeveloperTokenAuthStrategy>());
	// 	}
	// 	AssetApi->OnListAssetsResponse.BindRaw(this, &SPlayerZeroDeveloperLoginWidget::HandleBaseModelListResponse);
	// }
	if (!DeveloperAccountApi.IsValid())
	{
		DeveloperAccountApi = MakeShared<FDeveloperAccountApi>(nullptr);
		if (!DevAuthData.IsDemo)
		{
			DeveloperAccountApi->SetAuthenticationStrategy(MakeShared<DeveloperTokenAuthStrategy>());
		}
		// DeveloperAccountApi->OnOrganizationResponse.BindRaw(
		// 	this, &SPlayerZeroDeveloperLoginWidget::HandleOrganizationListResponse);
		// DeveloperAccountApi->OnApplicationListResponse.BindRaw(
		// 	this, &SPlayerZeroDeveloperLoginWidget::HandleApplicationListResponse);
	}
	bIsInitialized = true;
	if (bIsLoggedIn)
	{
		GetOrgList();
		return;
	}
	OnLogoutClicked();
}

SPlayerZeroDeveloperLoginWidget::~SPlayerZeroDeveloperLoginWidget()
{
	ClearLoadedCharacterModelImages();
}

void SPlayerZeroDeveloperLoginWidget::ClearLoadedCharacterModelImages()
{
	for (const auto Texture : CharacterStyleTextures)
	{
		Texture->RemoveFromRoot();
	}
	CharacterStyleTextures.Empty();
}

void SPlayerZeroDeveloperLoginWidget::AddCharacterStyle(const FAsset& StyleAsset)
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
					.OnClicked_Lambda([this, StyleAsset]() -> FReply
					{
						OnLoadBaseModelClicked(StyleAsset);
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
			   .Text(FText::FromString(FString::Printf(TEXT("ID: %s"), *StyleAsset.Id)))
			   .IsReadOnly(true)
			   .IsCaretMovedWhenGainFocus(false)
			   .SelectAllTextWhenFocused(false)
			   .MinDesiredWidth(100.0f)
		]
	];

	TSharedPtr<FPlayerZeroTextureLoader> ImageLoader = MakeShared<FPlayerZeroTextureLoader>();
	ActiveLoaders.Add(ImageLoader);
	ImageLoader->OnTextureLoaded.BindRaw(this, &SPlayerZeroDeveloperLoginWidget::OnTextureLoaded, ImageWidget, ImageLoader);
	ImageLoader->LoadIconFromAsset(StyleAsset);
}

void SPlayerZeroDeveloperLoginWidget::OnTextureLoaded(UTexture2D* Texture2D, TSharedPtr<SImage> SImage, TSharedPtr<FPlayerZeroTextureLoader> LoaderToRemove)
{
	if (Texture2D)
	{
		Texture2D->AddToRoot();
		CharacterStyleTextures.Add(Texture2D);
		FPlayerZeroImageHelper::LoadTextureToSImage(Texture2D, FVector2D(100.0f, 100.0f), SImage);
	}
	ActiveLoaders.Remove(LoaderToRemove);
}

void SPlayerZeroDeveloperLoginWidget::OnLoadBaseModelClicked(const FAsset& StyleAsset)
{
	// AssetLoader = MakeShared<FEditorAssetLoader>();
	// AssetLoader->LoadBaseModelAsset(StyleAsset);
}

EVisibility SPlayerZeroDeveloperLoginWidget::GetLoginViewVisibility() const
{
	return bIsLoggedIn ? EVisibility::Collapsed : EVisibility::Visible;
}

EVisibility SPlayerZeroDeveloperLoginWidget::GetLoggedInViewVisibility() const
{
	return bIsLoggedIn ? EVisibility::Visible : EVisibility::Collapsed;
}

FText SPlayerZeroDeveloperLoginWidget::GetWelcomeText() const
{
	return FText::Format(FText::FromString("Welcome {0}"), FText::FromString(UserName));
}

FReply SPlayerZeroDeveloperLoginWidget::OnLoginClicked()
{
	UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetMutableDefault<UPlayerZeroDeveloperSettings>();
	PlayerZeroSettings->Reset();
	FString Email = EmailTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();
	FEditorCache::SetString(CacheKeyEmail, Email);
	Email = Email.TrimStartAndEnd();
	Password = Password.TrimStartAndEnd();
	DeveloperAccountApi->SetAuthenticationStrategy(MakeShared<DeveloperTokenAuthStrategy>());
	// TODO update
	//AssetApi->SetAuthenticationStrategy(MakeShared<DeveloperTokenAuthStrategy>());
	FDeveloperLoginRequest LoginRequest = FDeveloperLoginRequest(Email, Password);
	DeveloperAuthApi->LoginWithEmail(LoginRequest);
	return FReply::Handled();
}

void SPlayerZeroDeveloperLoginWidget::GetOrgList()
{
	FOrganizationListRequest OrgRequest;
	DeveloperAccountApi->ListOrganizationsAsync(OrgRequest);
}

void SPlayerZeroDeveloperLoginWidget::HandleLoginResponse(const FDeveloperLoginResponse& Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UserName = Response.Data.Name;
		const FDeveloperAuth AuthData = FDeveloperAuth(Response.Data, false);
		FDevAuthTokenCache::SetAuthData(AuthData);
		SetLoggedInState(true);
		GetOrgList();
		return;
	}
	UE_LOG(LogPlayerZero, Error, TEXT("Login request failed"));
	FDevAuthTokenCache::ClearAuthData();
}

// void SPlayerZeroDeveloperLoginWidget::HandleOrganizationListResponse(const FOrganizationListResponse& Response, bool bWasSuccessful)
// {
// 	if (bWasSuccessful)
// 	{
// 		if (Response.Data.Num() == 0)
// 		{
// 			UE_LOG(LogPlayerZero, Error, TEXT("No organizations found"));
// 			return;
// 		}
// 		FApplicationListRequest Request;
// 		Request.Params.Add("organizationId", Response.Data[0].Id);
// 		DeveloperAccountApi->ListApplicationsAsync(Request);
// 		return;
// 	}
//
// 	UE_LOG(LogPlayerZero, Error, TEXT("Failed to list organizations"));
// }
//
//
// void SPlayerZeroDeveloperLoginWidget::HandleApplicationListResponse(const FApplicationListResponse& Response,                                                             bool bWasSuccessful)
// {
// 	if (bWasSuccessful)
// 	{
// 		const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
// 		UserApplications = Response.Data;
// 		FString Active;
// 		TArray<FString> Items;
// 		for (const FApplication& App : UserApplications)
// 		{
// 			Items.Add(App.Name);
// 			if (App.Id == PlayerZeroSettings->ApplicationId)
// 			{
// 				Active = App.Name;
// 			}
// 		}
// 		if (Active.IsEmpty() && Items.Num() > 0)
// 		{
// 			const auto NewActiveItem = MakeShared<FString>(Items[0]);
// 			OnComboBoxSelectionChanged(NewActiveItem, ESelectInfo::Direct);
// 			SelectedApplicationTextBlock->SetText(FText::FromString(*NewActiveItem));
// 		}
// 		PopulateComboBoxItems(Items, Active);
// 	}
// 	else
// 	{
// 		UE_LOG(LogPlayerZero, Error, TEXT("Failed to list applications"));
// 	}
// 	LoadBaseModelList();
// }


void SPlayerZeroDeveloperLoginWidget::PopulateComboBoxItems(const TArray<FString>& Items, const FString ActiveItem)
{
	ComboBoxItems.Empty();
	for (const FString& Item : Items)
	{
		ComboBoxItems.Add(MakeShared<FString>(Item));
	}
	SelectedComboBoxItem = MakeShared<FString>(ActiveItem);
}


FText SPlayerZeroDeveloperLoginWidget::GetSelectedComboBoxItemText() const
{
	return SelectedComboBoxItem.IsValid() && !SelectedComboBoxItem->IsEmpty()
		       ? FText::FromString(*SelectedComboBoxItem)
		       : FText::FromString("Select an option");
}


void SPlayerZeroDeveloperLoginWidget::OnComboBoxSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
{
	SelectedComboBoxItem = NewValue;
	FApplication* application = UserApplications.FindByPredicate([&](FApplication item)
	{
		return item.Name == *NewValue;
	});
	if (application)
	{
		UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetMutableDefault<UPlayerZeroDeveloperSettings>();
		PlayerZeroSettings->ApplicationId = application->Id;
		PlayerZeroSettings->SaveConfig();
	}
}


FReply SPlayerZeroDeveloperLoginWidget::OnUseDemoAccountClicked()
{
	UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetMutableDefault<UPlayerZeroDeveloperSettings>();
	PlayerZeroSettings->SetupDemoAccount();
	FDeveloperAuth AuthData = FDeveloperAuth();
	AuthData.Name = DemoUserName;
	AuthData.IsDemo = true;
	UserName = AuthData.Name;
	FDevAuthTokenCache::SetAuthData(AuthData);
	SetLoggedInState(true);

	// Unset the authentication strategy for the APIs
	DeveloperAccountApi->SetAuthenticationStrategy(nullptr);
	// TODO update
	//AssetApi->SetAuthenticationStrategy(nullptr);
	GetOrgList();
	return FReply::Handled();
}

FReply SPlayerZeroDeveloperLoginWidget::OnLogoutClicked()
{
	UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetMutableDefault<UPlayerZeroDeveloperSettings>();
	PlayerZeroSettings->Reset();

	// Clear the content box to remove all child widgets
	if (ContentBox.IsValid())
	{
		ContentBox->ClearChildren();
	}
	ComboBoxItems.Empty();

	ClearLoadedCharacterModelImages();
	FDevAuthTokenCache::ClearAuthData();
	SetLoggedInState(false);
	return FReply::Handled();
}

// void SPlayerZeroDeveloperLoginWidget::LoadBaseModelList()
// {
// 	const UPlayerZeroDeveloperSettings* PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
// 	if (PlayerZeroSettings->ApplicationId.IsEmpty())
// 	{
// 		UE_LOG(LogPlayerZero, Error, TEXT("Application ID is empty, unable to load base models."));
// 		return;
// 	}
// 	FAssetListRequest Request = FAssetListRequest();
// 	FAssetListQueryParams Params = FAssetListQueryParams();
// 	Params.ApplicationId = PlayerZeroSettings->ApplicationId;
// 	// TODO update
// 	// Params.Type = FAssetApi::BaseModelType;
// 	// Request.Params = Params;
// 	// AssetApi->ListAssetsAsync(Request);
// }

// void SPlayerZeroDeveloperLoginWidget::HandleBaseModelListResponse(const FAssetListResponse& Response, bool bWasSuccessful)
// {
// 	CharacterStyleAssets.Empty();
// 	for (FAsset Asset : Response.Data)
// 	{
// 		CharacterStyleAssets.Add(Asset.Id, Asset);
// 		AddCharacterStyle(Asset);
// 	}
// }


void SPlayerZeroDeveloperLoginWidget::SetLoggedInState(const bool IsLoggedIn)
{
	this->bIsLoggedIn = IsLoggedIn;

	// Force the UI to refresh
	Invalidate(EInvalidateWidget::Layout);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
