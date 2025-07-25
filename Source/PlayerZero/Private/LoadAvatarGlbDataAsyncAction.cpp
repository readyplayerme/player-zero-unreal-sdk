// Copyright Ready Player Me

#include "LoadAvatarGlbDataAsyncAction.h"
#include "PlayerZeroSubsystem.h"

ULoadAvatarGlbDataAsyncAction* ULoadAvatarGlbDataAsyncAction::LoadAvatarGlbDataAsync(UObject* WorldContextObject, const FString& Url)
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
		OnFailed.Broadcast();
		return;
	}

	if (const UWorld* World = ContextObject->GetWorld())
	{
		if (UPlayerZeroSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>())
		{
			Subsystem->DownloadAvatarData(CachedUrl, FOnAvatarDataDownloaded::CreateUObject(this, &ULoadAvatarGlbDataAsyncAction::OnDataDownloaded));
		}
	}
}

void ULoadAvatarGlbDataAsyncAction::OnDataDownloaded(const TArray<uint8>& Data)
{
	if (Data.Num() == 0)
	{
		OnFailed.Broadcast();
		return;
	}
	OnCompleted.Broadcast(Data);
}