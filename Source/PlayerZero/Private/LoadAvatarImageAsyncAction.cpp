// Copyright Ready Player Me

#include "LoadAvatarImageAsyncAction.h"
#include "PlayerZeroSubsystem.h"

ULoadAvatarImageAsyncAction* ULoadAvatarImageAsyncAction::LoadAvatarImageAsync(UObject* WorldContextObject, const FString& AvatarId)
{
	ULoadAvatarImageAsyncAction* Node = NewObject<ULoadAvatarImageAsyncAction>();
	Node->CachedAvatarId = AvatarId;
	Node->ContextObject = WorldContextObject;
	return Node;
}

void ULoadAvatarImageAsyncAction::Activate()
{
	if (!ContextObject)
	{
		OnFailed.Broadcast();
		return;
	}

	if (UWorld* World = ContextObject->GetWorld())
	{
		if (UPlayerZeroSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>())
		{
			Subsystem->GetAvatarIconAsTexture(CachedAvatarId, FOnAvatarTextureLoaded::CreateUObject(this, &ULoadAvatarImageAsyncAction::OnIconLoaded));
		}
	}
}

void ULoadAvatarImageAsyncAction::OnIconLoaded(UTexture2D* Texture)
{
	if (!Texture)
	{
		OnFailed.Broadcast();
		return;
	}
	OnCompleted.Broadcast(Texture);
}
