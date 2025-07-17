// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/PlayerZeroDeveloperSettings.h"

UPlayerZeroDeveloperSettings::UPlayerZeroDeveloperSettings() : ApiBaseUrl(TEXT("https://api.readyplayer.me")), ApiBaseAuthUrl(TEXT("https://readyplayer.me/api/auth")), DefaultAvatarId((TEXT("68304bfa19a6664919431f4e")))
{
	LoadConfig();
}

void UPlayerZeroDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();
}

void UPlayerZeroDeveloperSettings::PreSave(const ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);

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
