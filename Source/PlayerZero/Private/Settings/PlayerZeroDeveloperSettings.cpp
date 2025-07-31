// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/PlayerZeroDeveloperSettings.h"

UPlayerZeroDeveloperSettings::UPlayerZeroDeveloperSettings() : ApiBaseUrl(TEXT("https://api.readyplayer.me")), ApiBaseAuthUrl(TEXT("https://readyplayer.me/api/auth")), DefaultAvatarId((TEXT("688b3deec85bec2f31fd6d6d")))
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

	if(!ApplicationId.IsEmpty())
	{
		return;
	}
	
	// Ensure settings are saved before the build
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}

#if WITH_EDITOR
void UPlayerZeroDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
#endif // WITH_EDITOR

FString UPlayerZeroDeveloperSettings::GetApiBaseUrl() const
{
	return ApiBaseUrl;
}
