// Copyright Ready Player Me

#include "LoadAvatarGlbDataAsyncAction.h"
#include "PlayerZeroSubsystem.h"

ULoadAvatarGlbDataAsyncAction* ULoadAvatarGlbDataAsyncAction::Download(UObject* WorldContextObject, const FString& Url)
{
	ULoadAvatarGlbDataAsyncAction* Node = NewObject<ULoadAvatarGlbDataAsyncAction>();
	Node->CachedUrl = Url;
	Node->ContextObject = WorldContextObject;
	return Node;
}

void ULoadAvatarGlbDataAsyncAction::Activate()
{
	if (!ContextObject)
	{
		OnCompleted.Broadcast({});
		return;
	}

	if (UWorld* World = ContextObject->GetWorld())
	{
		if (UPlayerZeroSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>())
		{
			Subsystem->DownloadAvatarData(CachedUrl, FOnAvatarDataDownloaded::CreateUObject(this, &ULoadAvatarGlbDataAsyncAction::OnDataDownloaded));
		}
	}
}

void ULoadAvatarGlbDataAsyncAction::OnDataDownloaded(const TArray<uint8>& Data)
{
	OnCompleted.Broadcast(Data);
}