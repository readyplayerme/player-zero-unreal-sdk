#include "LoadAvatarMetaDataAsyncAction.h"
#include "PlayerZeroSubsystem.h"

ULoadAvatarMetaDataAsyncAction* ULoadAvatarMetaDataAsyncAction::Get(UObject* WorldContextObject, const FString& AvatarId)
{
	ULoadAvatarMetaDataAsyncAction* Node = NewObject<ULoadAvatarMetaDataAsyncAction>();
	Node->CachedAvatarId = AvatarId;
	Node->ContextObject = WorldContextObject;
	return Node;
}

void ULoadAvatarMetaDataAsyncAction::Activate()
{
	if (!ContextObject)
	{
		OnCompleted.Broadcast(FPlayerZeroCharacter());
		return;
	}

	if (UWorld* World = ContextObject->GetWorld())
	{
		if (UPlayerZeroSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>())
		{
			Subsystem->GetAvatarMetaData(CachedAvatarId, FOnCharacterDataLoaded::CreateUObject(this, &ULoadAvatarMetaDataAsyncAction::OnDataLoaded));
		}
	}
}

void ULoadAvatarMetaDataAsyncAction::OnDataLoaded(const FPlayerZeroCharacter& Character)
{
	OnCompleted.Broadcast(Character);
}