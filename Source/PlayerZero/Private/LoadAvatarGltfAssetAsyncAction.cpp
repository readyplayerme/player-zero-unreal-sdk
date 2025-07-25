// Copyright Ready Player Me

#include "LoadAvatarGltfAssetAsyncAction.h"
#include "PlayerZeroSubsystem.h"

ULoadAvatarGltfAssetAsyncAction* ULoadAvatarGltfAssetAsyncAction::LoadAvatarGltfAssetAsync(UObject* WorldContextObject, const FString& AvatarId)
{
	ULoadAvatarGltfAssetAsyncAction* Node = NewObject<ULoadAvatarGltfAssetAsyncAction>();
	Node->CachedAvatarId = AvatarId;
	Node->ContextObject = WorldContextObject;
	return Node;
}

void ULoadAvatarGltfAssetAsyncAction::Activate()
{
	if (!ContextObject)
	{
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (UWorld* World = ContextObject->GetWorld())
	{
		if (UPlayerZeroSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>())
		{
			Subsystem->LoadAvatarAsset(CachedAvatarId, FOnGltfAssetLoaded::CreateUObject(this, &ULoadAvatarGltfAssetAsyncAction::HandleCompleted));
		}
	}
}

void ULoadAvatarGltfAssetAsyncAction::HandleCompleted(UglTFRuntimeAsset* GltfAsset) const
{
	OnCompleted.Broadcast(GltfAsset);
}
