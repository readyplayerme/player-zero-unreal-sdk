// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PlayerZeroDeveloperSettings.generated.h"

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
class FObjectPreSaveContext;
#endif

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Player Zero Settings"))
class PLAYERZERO_API UPlayerZeroDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ReadOnly  = "true", ToolTip = "Base URL for requests."))
	FString ApiBaseUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ReadOnly  = "true", ToolTip = "Base URL for authentication requests."))
	FString ApiBaseAuthUrl;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Application ID used for authentication."))
	FString ApplicationId;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Game ID for the Player Zero application. This is used to identify the game in the Player Zero ecosystem."))
	FString GameId;

	UPROPERTY(EditAnywhere, Config, Category = "Player Zero Settings", meta = (ToolTip = "Default avatar ID for the Player Zero application. This when you try to load an avatar but no avatarId is set."))
	FString DefaultAvatarId;
	
	UPlayerZeroDeveloperSettings();
	
	FString GetApiBaseUrl() const;

	bool IsValid() const
	{
		return !ApplicationId.IsEmpty() && !GameId.IsEmpty() && !ApiBaseUrl.IsEmpty() && !ApiBaseAuthUrl.IsEmpty() && !DefaultAvatarId.IsEmpty();
	}

	virtual void PostInitProperties() override;
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 
};
