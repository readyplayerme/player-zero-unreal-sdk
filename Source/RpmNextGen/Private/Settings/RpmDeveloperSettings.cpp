// Copyright Ready Player Me

#include "Settings/RpmDeveloperSettings.h"

URpmDeveloperSettings::URpmDeveloperSettings() : ApiBaseUrl(TEXT("https://api.readyplayer.me")), ApiBaseAuthUrl(TEXT("https://readyplayer.me/api/auth"))
{
	LoadConfig();
}

void URpmDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();
}

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
void URpmDeveloperSettings::PreSave(FObjectPreSaveContext Context)
{
	Super::PreSave(Context);
#else
void URpmDeveloperSettings::PreSave(const ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);
#endif

	if(ApiKey.IsEmpty() && ApiProxyUrl.IsEmpty() && !ApplicationId.IsEmpty())
	{
		return;
	}
	
	// Ensure settings are saved before the build
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}

#if WITH_EDITOR
void URpmDeveloperSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
#endif // WITH_EDITOR

void URpmDeveloperSettings::SetupDemoAccount()
{
	ApplicationId = DemoAppId;
	ApiProxyUrl = DemoProxyUrl;
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}

void URpmDeveloperSettings::Reset()
{
	if(ApplicationId == DemoAppId)
	{
		ApplicationId = TEXT("");
	}
	if(ApiProxyUrl == DemoProxyUrl)
	{
		ApiProxyUrl = TEXT("");
	}
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}

FString URpmDeveloperSettings::GetApiBaseUrl() const
{
	return ApiProxyUrl.IsEmpty() ? ApiBaseUrl : ApiProxyUrl;
}
