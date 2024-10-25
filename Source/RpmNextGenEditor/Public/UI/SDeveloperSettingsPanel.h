#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SDeveloperSettingsPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDeveloperSettingsPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SVerticalBox> ContentBox;

	void PopulateSettingsContent();
};
