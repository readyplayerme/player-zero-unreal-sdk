#include "Subsystems/PlayerZeroAnalyticsSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Api/GameEvents/GameEventApi.h"
#include "Api/GameEvents/Events/AvatarSessionEndedEvent.h"
#include "Api/GameEvents/Events/AvatarSessionHeartbeatEvent.h"
#include "Api/GameEvents/Events/GameSessionEndedEvent.h"
#include "Api/GameEvents/Events/GameSessionStartedEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ConfigCacheIni.h"
#include "Settings/PlayerZeroDeveloperSettings.h"

class UPlayerZeroSubsystem;

void UPlayerZeroAnalyticsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LastPlayerActivity = FDateTime::UtcNow();
	PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	// TODO: Replace with real hot-loaded ID check
	if (true )
	{
		StartSessions();
	}
}

void UPlayerZeroAnalyticsSubsystem::Deinitialize()
{
	Super::Deinitialize();
	EndSessions();
	PlayerZeroSettings = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(HeartbeatTimerHandle);
	HeartbeatActive = false;
}

void UPlayerZeroAnalyticsSubsystem::StartSessions()
{
	AvatarSessionId = GameSessionId = FGuid::NewGuid().ToString();

	GConfig->SetString(
		TEXT("/Script/PlayerZero.PlayerZeroAnalytics"),
		TEXT("PZ_SESSION_ID"),
		*GameSessionId,
		GGameIni
	);

	GConfig->SetString(
		TEXT("/Script/PlayerZero.PlayerZeroAnalytics"),
		TEXT("PZ_AVATAR_SESSION_ID"),
		*AvatarSessionId,
		GGameIni
	);

	GConfig->Flush(false, GGameIni); // force write to disk immediately
	GameSessionStart();
	AvatarSessionStart();
	StartHeartbeat();
}

void UPlayerZeroAnalyticsSubsystem::StartHeartbeat()
{
	if (!HeartbeatActive)
	{
		GetWorld()->GetTimerManager().SetTimer(HeartbeatTimerHandle, this, &UPlayerZeroAnalyticsSubsystem::SendHeartbeat, 60.f, true);
		HeartbeatActive = true;
	}
}

void UPlayerZeroAnalyticsSubsystem::SendHeartbeat()
{
	if (AvatarSessionId.IsEmpty())
		return;

	// Send a heartbeat event
	FAvatarSessionHeartbeatProperties Props;
	Props.SessionId = AvatarSessionId;
	Props.LastAvatarActivityAt = LastPlayerActivity.ToUnixTimestamp() * 1000;

	FAvatarSessionHeartbeat HeartbeatEvent;
	HeartbeatEvent.Properties = Props;

	auto Api = MakeShared<FGameEventApi>();
	Api->SendGameEventAsync(HeartbeatEvent, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
	{
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("Heartbeat Event sent: %s"), *Response);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Heartbeat Event failed: %s"), *Response);
		}
	}));
}

void UPlayerZeroAnalyticsSubsystem::GameSessionStart()
{	
	FGameSessionStartedEventProperties Props;
	Props.SessionId = GameSessionId;
	Props.GameId = PlayerZeroSettings->GameId; // Replace with actual game ID
	Props.AvatarId = "AvatarId"; // Replace with actual avatar ID
	Props.ApplicationId = PlayerZeroSettings->ApplicationId; 
	Props.ProductName = "ProductName"; // Replace with actual product name
	FGameSessionStartedEvent Event;
	Event.Properties = Props;
	
	auto Api = MakeShared<FGameEventApi>();
	Api->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
	{
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("GameSessionStart Event sent: %s"), *Response);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameSessionStart Event failed: %s"), *Response);
		}
	}));
}

void UPlayerZeroAnalyticsSubsystem::GameSessionEnd()
{
	FGameSessionEndedEventProperties Props;
	Props.SessionId = GameSessionId;
	Props.GameId = PlayerZeroSettings->GameId;
	
	FGameSessionEndedEvent Event;
	Event.Properties = Props;
	auto Api = MakeShared<FGameEventApi>();
	Api->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
	{
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("GameSessionEnd Event sent: %s"), *Response);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameSessionEnd Event failed: %s"), *Response);
		}
	}));
}

void UPlayerZeroAnalyticsSubsystem::AvatarSessionStart()
{
	FGameSessionStartedEventProperties Props;
	Props.SessionId = AvatarSessionId;
	Props.GameId = PlayerZeroSettings->GameId;
	Props.AvatarId = "AvatarId"; // TODO Replace with actual avatar ID
	Props.ApplicationId = PlayerZeroSettings->ApplicationId;
	Props.ProductName = "ProductName"; // TODO Replace with actual product name
	FGameSessionStartedEvent Event;
	Event.Properties = Props;
	auto Api = MakeShared<FGameEventApi>();
	Api->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
	{
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("AvatarSessionStart Event sent: %s"), *Response);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AvatarSessionStart Event failed: %s"), *Response);
		}
	}));
}

void UPlayerZeroAnalyticsSubsystem::AvatarSessionEnd()
{
	FAvatarSessionEndedProperties Props;
	Props.SessionId = AvatarSessionId;
	Props.GameId = PlayerZeroSettings->GameId;
	
	FAvatarSessionEnded Event;
	Event.Properties = Props;

	auto Api = MakeShared<FGameEventApi>();
	Api->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
	{
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("AvatarSessionEnd Event sent: %s"), *Response);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AvatarSessionEnd Event failed: %s"), *Response);
		}
	}));
}

void UPlayerZeroAnalyticsSubsystem::EndSessions()
{
	if (!AvatarSessionId.IsEmpty())
	{
		FAvatarSessionEndedProperties Props;
		Props.SessionId = AvatarSessionId;
		Props.GameId = PlayerZeroSettings->GameId;
		FAvatarSessionEnded Event;
		Event.Properties = Props;

		auto Api = MakeShared<FGameEventApi>();
		Api->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
		{
			if ( bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("AvatarSessionEnded sent: %s"), *Response);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AvatarSessionEnded failed: %s"), *Response);
			}
		}));
		AvatarSessionId.Empty();
	}

	if (!GameSessionId.IsEmpty())
	{
		FGameSessionEndedEventProperties Props;
		Props.SessionId = GameSessionId;
		Props.GameId = PlayerZeroSettings->GameId;
		FGameSessionEndedEvent Event;
		Event.Properties = Props;

		auto Api = MakeShared<FGameEventApi>();
		Api->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
		{
			if ( bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("GameSessionEnded sent: %s"), *Response);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GameSessionEnded failed: %s"), *Response);
			}
		}));
		GameSessionId.Empty();
	}
}

bool UPlayerZeroAnalyticsSubsystem::DetectPlayerActivity()
{
	// Example legacy detection — expand with input system or polling
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FVector2D CurrentMousePosition;
		PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);

		if (CurrentMousePosition != LastMousePosition)
		{
			LastMousePosition = CurrentMousePosition;
			LastPlayerActivity = FDateTime::UtcNow();
			return true;
		}
	}
	return false;
}
