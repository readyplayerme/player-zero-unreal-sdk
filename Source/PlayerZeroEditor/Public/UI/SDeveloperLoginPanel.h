﻿#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FApplicationListResponse;
struct FOrganizationListResponse;
class FDeveloperAccountApi;
struct FDeveloperLoginResponse;
class FDeveloperAuthApi;

DECLARE_DELEGATE_OneParam(FOnLoginSuccess, const FString&);

class RPMNEXTGENEDITOR_API SDeveloperLoginPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDeveloperLoginPanel) {}
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
	
	FOnLoginSuccess OnLoginSuccess;
private:
	TSharedPtr<FDeveloperAuthApi> DeveloperAuthApi;
	TSharedPtr<SEditableTextBox> EmailTextBox;
	TSharedPtr<SEditableTextBox> PasswordTextBox;
	TSharedPtr<STextBlock> EmailErrorText;
	TSharedPtr<STextBlock> PasswordErrorText;
	TSharedPtr<STextBlock> LoginErrorText;
	static constexpr const TCHAR* CacheKeyEmail = TEXT("RPM-Email");
	bool bShowErrorMessage = false;
	bool bFailedLogin = false;
	FReply OnLoginClicked();
	FReply OnUseDemoAccountClicked();

	void Initialize();
	void HandleLoginResponse(TSharedPtr<FDeveloperLoginResponse>, bool bWasSuccessful);
	void OnTextChanged(const FText& Text);
	void UpdateErrorMessages();
	bool IsInputTextValid() const;
};
