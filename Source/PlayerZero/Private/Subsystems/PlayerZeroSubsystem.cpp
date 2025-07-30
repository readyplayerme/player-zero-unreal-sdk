#include "Subsystems/PlayerZeroSubsystem.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "PlayerZero.h"
#include "PlayerZeroConfigProcessor.h"
#include "Api/Characters/CharacterApi.h"
#include "Api/Characters/Models/CharacterFindByIdRequest.h"
#include "Api/Files/GlbLoader.h"
#include "Api/Files/Models/FileData.h"
#include "Api/GameEvents/Events/AvatarSessionEndedEvent.h"
#include "Api/GameEvents/Events/AvatarSessionHeartbeatEvent.h"
#include "Api/GameEvents/Events/GameSessionEndedEvent.h"
#include "Api/GameEvents/Events/GameSessionStartedEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/PlayerZeroDeveloperSettings.h"
#include "Utilities/PlayerZeroImageHelper.h"

void UPlayerZeroSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FileApi = MakeShared<FFileApi>();
	CharacterApi = MakeShared<FCharacterApi>();
	GameEventApi = MakeShared<FGameEventApi>();
	LastPlayerActivity = FDateTime::UtcNow();
	PlayerZeroSettings = GetDefault<UPlayerZeroDeveloperSettings>();
	// TODO: Replace with real hot-loaded ID check
	if (true )
	{
		StartSessions();
	}
}

void UPlayerZeroSubsystem::Deinitialize()
{
	Super::Deinitialize();
	EndSessions();
	PlayerZeroSettings = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(HeartbeatTimerHandle);
	HeartbeatActive = false;
}

FString UPlayerZeroSubsystem::GetHotLoadedAvatarId()
{
	if (bIsInitialized)
	{
		return CachedAvatarId;
	}
	
	// Initialize the SDK and set the CachedAvatarId
	bIsInitialized = true;
	CachedAvatarId = TEXT("default_avatar_id"); // Replace with actual logic to get the avatar ID
	return CachedAvatarId;
}

void UPlayerZeroSubsystem::GetAvatarIconAsTexture(FString AvatarId, const FAvatarRenderConfig& Config, FOnAvatarTextureLoaded OnComplete)
{
	const FString Url = FString::Printf(TEXT("https://avatars.readyplayer.me/%s.png%s"), *AvatarId, *FPlayerZeroConfigProcessor::ProcessRender(Config));

	TWeakObjectPtr<UPlayerZeroSubsystem> WeakThis(this);
	FileApi->LoadFileFromUrl(Url, FOnAssetFileRequestComplete::CreateLambda(
	[WeakThis, OnComplete](const FFileData& File, const TArray<uint8>& Data)
	{
		if (!WeakThis.IsValid())
		{
			return;
		}
		if (UTexture2D* Texture = FPlayerZeroImageHelper::CreateTextureFromData(Data))
		{
			OnComplete.ExecuteIfBound(Texture);
			return; 
		}
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to load icon from url: %s"), *File.Url);
		OnComplete.ExecuteIfBound(nullptr);
	}));
}

void UPlayerZeroSubsystem::GetAvatarMetaData(const FString& Id, FOnCharacterDataLoaded OnComplete)
{
	TSharedPtr<FCharacterFindByIdRequest> Request = MakeShared<FCharacterFindByIdRequest>();
	Request->Id = Id;
	TWeakObjectPtr<UPlayerZeroSubsystem> WeakThis(this);
	CharacterApi->FindByIdAsync(*Request, FOnCharacterFindResponse::CreateLambda(
		[WeakThis, OnComplete](FCharacterFindByIdResponse Response, bool bWasSuccessful)
		{
			if (!WeakThis.IsValid())
			{
				return;
			}
			if (bWasSuccessful)
			{
				OnComplete.ExecuteIfBound(Response.Data);
			}
			else
			{
				OnComplete.ExecuteIfBound(FPlayerZeroCharacter());
			}
		}));
}

void UPlayerZeroSubsystem::LoadAvatarAsset(const FString& Id, const FCharacterConfig& Config, const FOnGltfAssetLoaded& OnComplete)
{
	// Step 1: Fetch character metadata
	TSharedPtr<FCharacterFindByIdRequest> Request = MakeShared<FCharacterFindByIdRequest>();
	Request->Id = Id;
 	TWeakObjectPtr<UPlayerZeroSubsystem> WeakThis(this);
	CharacterApi->FindByIdAsync(*Request, FOnCharacterFindResponse::CreateLambda(
		[WeakThis, Config, OnComplete](FCharacterFindByIdResponse Response, bool bCharacterFetchSuccessful)
		{
			if (!WeakThis.IsValid())
			{
				return;
			}
			if (!bCharacterFetchSuccessful || Response.Data.Id.IsEmpty())
			{
				UE_LOG(LogPlayerZero, Error, TEXT("Failed to find character or ID was empty"));
				OnComplete.ExecuteIfBound(nullptr);
				return;
			}

			const FString GlbUrl = Response.Data.ModelUrl + FPlayerZeroConfigProcessor::ProcessCharacter(Config);
			if (GlbUrl.IsEmpty())
			{
				UE_LOG(LogPlayerZero, Error, TEXT("Character GLB URL is empty"));
				OnComplete.ExecuteIfBound(nullptr);
				return;
			}
			
			WeakThis->FileApi->LoadFileFromUrl(GlbUrl, FOnAssetFileRequestComplete::CreateLambda(
				[WeakThis, OnComplete](const FFileData& File, const TArray<uint8>& Data)
				{
					if (!WeakThis.IsValid())
					{
						return;
					}
					if (Data.Num() == 0)
					{
						UE_LOG(LogPlayerZero, Error, TEXT("Downloaded GLB data is empty"));
						OnComplete.ExecuteIfBound(nullptr);
						return;
					}
					// Step 3: Load the GLTF asset
					FglTFRuntimeConfig Config;
					Config.TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
					UglTFRuntimeAsset* GltfAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, Config);
					
					if (!GltfAsset)
					{
						UE_LOG(LogPlayerZero, Error, TEXT("Failed to load GLTF asset from data"));
					}
					OnComplete.ExecuteIfBound(GltfAsset);
				}));
		}));
}

void UPlayerZeroSubsystem::DownloadAvatarData(const FString& Url, const FCharacterConfig& Config, FOnAvatarDataDownloaded OnComplete)
{
	const FString ProcessedUrlParams = Url + FPlayerZeroConfigProcessor::ProcessCharacter(Config);
	TWeakObjectPtr<UPlayerZeroSubsystem> WeakThis(this);
	FileApi->LoadFileFromUrl(ProcessedUrlParams, FOnAssetFileRequestComplete::CreateLambda(
		[WeakThis, OnComplete](const FFileData& File, const TArray<uint8>& Data)
		{
			if (!WeakThis.IsValid())
			{
				return;
			}
			OnComplete.ExecuteIfBound(Data);
		}));
}


void UPlayerZeroSubsystem::LoadGltfAsset(const TArray<uint8>& Data, const FglTFRuntimeConfig& Config, FOnGltfAssetLoaded OnComplete)
{
	UglTFRuntimeAsset* Asset = nullptr;
	if (Data.Num() > 0)
	{
		Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, Config);
	}
	else
	{
		UE_LOG(LogPlayerZero, Error, TEXT("GLTF data is empty."));
	}
	OnComplete.ExecuteIfBound(Asset);
}

void UPlayerZeroSubsystem::StartSessions()
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

void UPlayerZeroSubsystem::StartHeartbeat()
{
	if (!HeartbeatActive)
	{
		GetWorld()->GetTimerManager().SetTimer(HeartbeatTimerHandle, this, &UPlayerZeroSubsystem::SendHeartbeat, 60.f, true);
		HeartbeatActive = true;
	}
}

void UPlayerZeroSubsystem::SendHeartbeat()
{
	if (AvatarSessionId.IsEmpty())
		return;

	FAvatarSessionHeartbeatProperties Props;
	Props.SessionId = AvatarSessionId;
	Props.LastAvatarActivityAt = LastPlayerActivity.ToUnixTimestamp() * 1000;

	FAvatarSessionHeartbeat HeartbeatEvent;
	HeartbeatEvent.Properties = Props;

	GameEventApi->SendGameEventAsync(HeartbeatEvent, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
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

void UPlayerZeroSubsystem::GameSessionStart()
{	
	FGameSessionStartedEventProperties Props;
	Props.SessionId = GameSessionId;
	Props.GameId = PlayerZeroSettings->GameId; 
	Props.AvatarId = "AvatarId"; // Replace with actual avatar ID
	Props.ApplicationId = PlayerZeroSettings->ApplicationId; 
	Props.ProductName = "ProductName"; 
	FGameSessionStartedEvent Event;
	Event.Properties = Props;
	
	GameEventApi->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
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

void UPlayerZeroSubsystem::GameSessionEnd()
{
	FGameSessionEndedEventProperties Props;
	Props.SessionId = GameSessionId;
	Props.GameId = PlayerZeroSettings->GameId;
	
	FGameSessionEndedEvent Event;
	Event.Properties = Props;

	GameEventApi->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
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

void UPlayerZeroSubsystem::AvatarSessionStart()
{
	FGameSessionStartedEventProperties Props;
	Props.SessionId = AvatarSessionId;
	Props.GameId = PlayerZeroSettings->GameId;
	Props.AvatarId = "AvatarId"; // TODO Replace with actual avatar ID
	Props.ApplicationId = PlayerZeroSettings->ApplicationId;
	Props.ProductName = "ProductName"; // TODO Replace with actual product name
	FGameSessionStartedEvent Event;
	Event.Properties = Props;
	
	GameEventApi->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
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

void UPlayerZeroSubsystem::AvatarSessionEnd()
{
	FAvatarSessionEndedProperties Props;
	Props.SessionId = AvatarSessionId;
	Props.GameId = PlayerZeroSettings->GameId;
	
	FAvatarSessionEnded Event;
	Event.Properties = Props;

	
	GameEventApi->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
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

void UPlayerZeroSubsystem::EndSessions()
{
	if (!AvatarSessionId.IsEmpty())
	{
		FAvatarSessionEndedProperties Props;
		Props.SessionId = AvatarSessionId;
		Props.GameId = PlayerZeroSettings->GameId;
		FAvatarSessionEnded Event;
		Event.Properties = Props;

		GameEventApi->SendGameEventAsync(Event, FOnGameEventSent::CreateLambda([](bool bSuccess, const FString& Response)
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

bool UPlayerZeroSubsystem::DetectPlayerActivity()
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

void UPlayerZeroSubsystem::OnDeepLinkDataReceived(const FString& AvatarId)
{
}
