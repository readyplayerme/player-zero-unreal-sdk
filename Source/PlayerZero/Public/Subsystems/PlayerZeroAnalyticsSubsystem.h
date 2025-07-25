#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerZeroAnalyticsSubsystem.generated.h"

class UPlayerZeroDeveloperSettings;
class FGameEventApi;

UCLASS()
class PLAYERZERO_API UPlayerZeroAnalyticsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void StartSessions();
	void StartHeartbeat();
	void GameSessionStart();
	void GameSessionEnd();
	void AvatarSessionStart();
	void AvatarSessionEnd();
	void EndSessions();
private:
	const UPlayerZeroDeveloperSettings* PlayerZeroSettings;
	FTimerHandle HeartbeatTimerHandle;
	FDateTime LastPlayerActivity;
	FVector2D LastMousePosition;
	FString AvatarSessionId;
	FString GameSessionId;
	bool HeartbeatActive = false;
	
	void SendHeartbeat();
	bool DetectPlayerActivity();
};