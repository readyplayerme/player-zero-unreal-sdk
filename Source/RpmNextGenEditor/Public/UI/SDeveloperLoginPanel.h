#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FOrganizationListResponse;
class FDeveloperAccountApi;
struct FDeveloperLoginResponse;
class FDeveloperAuthApi;

DECLARE_DELEGATE_OneParam(FOnLoginSuccess, const FString&);
DECLARE_DELEGATE_OneParam(FOnOrgRequestComplete, const FString&);

class SDeveloperLoginPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDeveloperLoginPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<FDeveloperAuthApi> DeveloperAuthApi;
	TSharedPtr<FDeveloperAccountApi> DeveloperAccountApi;
	
	FOnLoginSuccess OnLoginSuccess;
	FOnOrgRequestComplete OnOrgRequestComplete;
	
	TSharedPtr<SEditableTextBox> EmailTextBox;
	TSharedPtr<SEditableTextBox> PasswordTextBox;
	static constexpr const TCHAR* CacheKeyEmail = TEXT("RPM-Email");
	FReply OnLoginClicked();
	
	void Initialize();
	void HandleLoginResponse(TSharedPtr<FDeveloperLoginResponse>, bool bWasSuccessful);
	void GetOrganizationList();
	void HandleOrganizationListResponse(TSharedPtr<FOrganizationListResponse> Response, bool bWasSuccessful);
};
