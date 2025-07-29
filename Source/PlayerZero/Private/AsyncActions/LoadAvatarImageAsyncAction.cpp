// Copyright Ready Player Me

#include "AsyncActions/LoadAvatarImageAsyncAction.h"
#include "Subsystems/PlayerZeroSubsystem.h"

ULoadAvatarImageAsyncAction* ULoadAvatarImageAsyncAction::LoadAvatarImageAsync(UObject* WorldContextObject, const FString& AvatarId, const FAvatarRenderConfig& Config)
{
	ULoadAvatarImageAsyncAction* Node = NewObject<ULoadAvatarImageAsyncAction>();
	Node->CachedAvatarId = AvatarId;
	Node->ContextObject = WorldContextObject;
	Node->Config = Config;
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
			Subsystem->GetAvatarIconAsTexture(CachedAvatarId, Config, FOnAvatarTextureLoaded::CreateUObject(this, &ULoadAvatarImageAsyncAction::OnIconLoaded));
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
