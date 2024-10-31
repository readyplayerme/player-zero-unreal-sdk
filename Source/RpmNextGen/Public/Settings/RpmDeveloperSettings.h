// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RpmDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Ready Player Me"))
class RPMNEXTGEN_API URpmDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Config, Category = "Auth Settings", meta = (ReadOnly  = "true", ToolTip = "Base URL for requests."))
	FString ApiBaseUrl;

public:
	
	UPROPERTY(VisibleAnywhere, Config, Category = "Auth Settings", meta = (ReadOnly  = "true", ToolTip = "Base URL for authentication requests."))
	FString ApiBaseAuthUrl;

	UPROPERTY(EditAnywhere, Config, Category = "Auth Settings", meta = (ToolTip = "Application ID used for authentication."))
	FString ApplicationId;

	UPROPERTY(EditAnywhere, Config, Category = "Auth Settings", meta = (ToolTip = "API key used for authentication."))
	FString ApiKey;
	
	UPROPERTY(EditAnywhere, Config, Category = "Auth Settings", meta = (ToolTip = "Proxy URL for API requests. If empty, the base URL will be used."))
	FString ApiProxyUrl;
	
	URpmDeveloperSettings();
	
	void SetupDemoAccount();
	void Reset();
	
	FString GetApiBaseUrl() const;

	bool IsValid() const
	{
		return !ApplicationId.IsEmpty() && (!ApiKey.IsEmpty() || !ApiProxyUrl.IsEmpty());
	}

	virtual void PostInitProperties() override;
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
#else
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif
private:
	const FString DemoAppId = TEXT("665e05a50c62c921e5a6ab84");
	const FString DemoProxyUrl = TEXT("https://api.readyplayer.me/demo");
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 
};
