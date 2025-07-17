// Copyright Ready Player Me

#include "Settings/PlayerZeroDeveloperSettings.h"

<<<<<<< HEAD:Source/PlayerZero/Private/Settings/PlayerZeroDeveloperSettings.cpp
UPlayerZeroDeveloperSettings::UPlayerZeroDeveloperSettings() : ApiBaseUrl(TEXT("https://api.readyplayer.me")), ApiBaseAuthUrl(TEXT("https://readyplayer.me/api/auth"))
=======
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
#include "UObject/ObjectSaveContext.h"
#endif


URpmDeveloperSettings::URpmDeveloperSettings() : ApiBaseUrl(TEXT("https://api.readyplayer.me")), ApiBaseAuthUrl(TEXT("https://readyplayer.me/api/auth"))
>>>>>>> origin/develop:Source/RpmNextGen/Private/Settings/RpmDeveloperSettings.cpp
{
	LoadConfig();
}

void UPlayerZeroDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();
}

<<<<<<< HEAD:Source/PlayerZero/Private/Settings/PlayerZeroDeveloperSettings.cpp
void UPlayerZeroDeveloperSettings::PreSave(const ITargetPlatform* TargetPlatform)
=======
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
void URpmDeveloperSettings::PreSave(FObjectPreSaveContext Context)
{
	Super::PreSave(Context);
#else
void URpmDeveloperSettings::PreSave(const ITargetPlatform* TargetPlatform)
>>>>>>> origin/develop:Source/RpmNextGen/Private/Settings/RpmDeveloperSettings.cpp
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
void UPlayerZeroDeveloperSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
#endif // WITH_EDITOR

<<<<<<< HEAD:Source/PlayerZero/Private/Settings/PlayerZeroDeveloperSettings.cpp

void UPlayerZeroDeveloperSettings::SetupDemoAccount()
=======
void URpmDeveloperSettings::SetupDemoAccount()
>>>>>>> origin/develop:Source/RpmNextGen/Private/Settings/RpmDeveloperSettings.cpp
{
	ApplicationId = DemoAppId;
	ApiProxyUrl = DemoProxyUrl;
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}

void UPlayerZeroDeveloperSettings::Reset()
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

FString UPlayerZeroDeveloperSettings::GetApiBaseUrl() const
{
	return ApiProxyUrl.IsEmpty() ? ApiBaseUrl : ApiProxyUrl;
}
