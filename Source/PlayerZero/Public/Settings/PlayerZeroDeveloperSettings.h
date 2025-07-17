// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PlayerZeroDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Player Zero Settings"))
class PLAYERZERO_API UPlayerZeroDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Config, Category = "Player Zero Settings", meta = (ReadOnly  = "true", ToolTip = "Base URL for requests."))
	FString ApiBaseUrl;

public:
	
	UPROPERTY(VisibleAnywhere, Config, Category = "Player Zero Settings", meta = (ReadOnly  = "true", ToolTip = "Base URL for authentication requests."))
	FString ApiBaseAuthUrl;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Application ID used for authentication."))
	FString ApplicationId;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "API key used for authentication."))
	FString ApiKey;
	
	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Proxy URL for API requests. If empty, the base URL will be used."))
	FString ApiProxyUrl;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Game ID for the Player Zero application. This is used to identify the game in the Player Zero ecosystem."))
	FString GameId;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Default avatar ID for the Player Zero application. This when you try to load an avatar but no avatarId is set."))
	FString DefaultAvatarId;
	
	UPlayerZeroDeveloperSettings();
	
	void SetupDemoAccount();
	void Reset();
	
	FString GetApiBaseUrl() const;

	bool IsValid() const
	{
		return !ApplicationId.IsEmpty() && (!ApiKey.IsEmpty() || !ApiProxyUrl.IsEmpty());
	}

	virtual void PostInitProperties() override;
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
private:
	const FString DemoAppId = TEXT("665e05a50c62c921e5a6ab84");
	const FString DemoProxyUrl = TEXT("https://api.readyplayer.me/demo");
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 
};
