// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerZeroEditor.h"
#include "UI/SCharacterLoaderWidget.h"
#include "UI/Commands/LoaderWindowCommands.h"
#include "UI/Commands/LoginWindowCommands.h"
#include "UI/SPlayerZeroDeveloperLoginWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "UI/LoginWindowStyle.h"

static const FName DeveloperWindowName("LoginWindow");
static const FName LoaderWindowName("LoaderWindow");
static const FName CacheWindowName("CacheGeneratorWindow");
#define LOCTEXT_NAMESPACE "PlayerZeroEditorModule"


void FPlayerZeroEditorModule::StartupModule()
{
	FLoginWindowStyle::Initialize();
	FLoginWindowStyle::ReloadTextures();

	FLoginWindowCommands::Register();
	FLoaderWindowCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLoginWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPlayerZeroEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	// Don't show Loader window in the menu
	// PluginCommands->MapAction(
	// 	FLoaderWindowCommands::Get().OpenPluginWindow,
	// 	FExecuteAction::CreateRaw(this, &FPlayerZeroEditorModule::OpenLoaderWindow),
	// 	FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPlayerZeroEditorModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DeveloperWindowName, FOnSpawnTab::CreateRaw(this, &FPlayerZeroEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("DeveloperLoginWidget", "Ready Player Me"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	
	// Don't show Loader window in the menu
	// FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NewWindowTabName, FOnSpawnTab::CreateRaw(this, &FPlayerZeroEditorModule::OnSpawnLoaderWindow))
	// 	.SetDisplayName(LOCTEXT("CharacterLoaderWidget", "Avatar Loader"))
	// 	.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPlayerZeroEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	// Create a new main menu entry called "ReadyPlayerMe"
	UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");

	// Add a new top-level menu "Ready Player Me"
	FToolMenuSection& Section = MainMenu->AddSection("ReadyPlayerMeTopMenu", LOCTEXT("ReadyPlayerMeMenuSection", "Ready Player Me"));

	// Add a sub-menu for "Ready Player Me"
	FToolMenuEntry& SubMenuEntry = Section.AddSubMenu(
		"ReadyPlayerMe",
		LOCTEXT("ReadyPlayerMeMenu", "Ready Player Me"),
		LOCTEXT("ReadyPlayerMeMenu_ToolTip", "Open Ready Player Me tools"),
		FNewToolMenuDelegate::CreateRaw(this, &FPlayerZeroEditorModule::FillReadyPlayerMeMenu),
		false, // Don't open on hover
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.User") // Optional icon for the top-level menu
	);
}

void FPlayerZeroEditorModule::FillReadyPlayerMeMenu(UToolMenu* Menu)
{
	FToolMenuSection& Section = Menu->AddSection("ReadyPlayerMeSubSection");

	Section.AddMenuEntry(
		"OpenLoginWindow",
		LOCTEXT("OpenLoginWindow", "Developer Window"),
		LOCTEXT("OpenLoginWindowToolTip", "Open the RPM Developer Window."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FPlayerZeroEditorModule::PluginButtonClicked))
	);
	
	// Don't show Loader window in the menu
	// Section.AddMenuEntry(
	// 	"OpenLoaderWindow",
	// 	LOCTEXT("OpenLoaderWindow", "Glb Loader"),
	// 	LOCTEXT("OpenLoaderWindowToolTip", "Avatar Loader Window."),
	// 	FSlateIcon(),
	// 	FUIAction(FExecuteAction::CreateRaw(this, &FPlayerZeroEditorModule::OpenLoaderWindow))
	// );
}

void FPlayerZeroEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FLoginWindowStyle::Shutdown();

	FLoginWindowCommands::Unregister();
	FLoaderWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DeveloperWindowName);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CacheWindowName);
	
	// Don't show Loader window in the menu
	//FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LoaderWindowName);
}

TSharedRef<SDockTab> FPlayerZeroEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FPlayerZeroEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("PlayerZeroEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SPlayerZeroDeveloperLoginWidget)
		];
	
}

TSharedRef<SDockTab> FPlayerZeroEditorModule::OnSpawnLoaderWindow(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SCharacterLoaderWidget)
		];
}

void FRpmNextGenEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(DeveloperWindowName);
}


void FPlayerZeroEditorModule::OpenLoaderWindow()
{
	FGlobalTabmanager::Get()->TryInvokeTab(LoaderWindowName);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPlayerZeroEditorModule, PlayerZeroEditor)