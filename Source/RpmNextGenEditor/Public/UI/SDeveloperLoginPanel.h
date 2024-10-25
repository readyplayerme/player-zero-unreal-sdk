#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FApplicationListResponse;
struct FOrganizationListResponse;
class FDeveloperAccountApi;
struct FDeveloperLoginResponse;
class FDeveloperAuthApi;

DECLARE_DELEGATE_OneParam(FOnLoginSuccess, const FString&);
DECLARE_DELEGATE_OneParam(FOnApplicationsRequested, const FApplicationListResponse&);

class SDeveloperLoginPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDeveloperLoginPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void HandleApplicationListResponse(TSharedPtr<FApplicationListResponse> Response, bool bWasSuccessful);

private:
	TSharedPtr<FDeveloperAuthApi> DeveloperAuthApi;
	TSharedPtr<FDeveloperAccountApi> DeveloperAccountApi;
	
	FOnLoginSuccess OnLoginSuccess;
	FOnApplicationsRequested OnOrgRequestComplete;
	
	TSharedPtr<SEditableTextBox> EmailTextBox;
	TSharedPtr<SEditableTextBox> PasswordTextBox;
	static constexpr const TCHAR* CacheKeyEmail = TEXT("RPM-Email");
	
	FReply OnLoginClicked();
	
	void Initialize();
	void HandleLoginResponse(TSharedPtr<FDeveloperLoginResponse>, bool bWasSuccessful);
	void GetOrganizationList();
};
