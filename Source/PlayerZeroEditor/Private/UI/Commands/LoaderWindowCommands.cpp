#include "UI/Commands/LoaderWindowCommands.h"

#define LOCTEXT_NAMESPACE "FPlayerZeroEditorModule"

void FLoaderWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Open Loader Window", "Open the custom loader window", EUserInterfaceActionType::Button, FInputGesture());
}
#undef LOCTEXT_NAMESPACE