// Copyright Ready Player Me

#include "LoadGltfFromDataAsyncAction.h"
#include "PlayerZeroSubsystem.h"

ULoadGltfFromDataAsyncAction* ULoadGltfFromDataAsyncAction::LoadGltfFromDataAsync(UObject* WorldContextObject, const TArray<uint8>& GltfBinaryData, const FglTFRuntimeConfig& Config)
{
	ULoadGltfFromDataAsyncAction* Node = NewObject<ULoadGltfFromDataAsyncAction>();
	Node->ContextObject = WorldContextObject;
	Node->Data = GltfBinaryData;
	Node->RuntimeConfig = Config;
	return Node;
}


void ULoadGltfFromDataAsyncAction::Activate()
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
			Subsystem->LoadGltfAsset(Data, RuntimeConfig, FOnGltfAssetLoaded::CreateUObject(this, &ULoadGltfFromDataAsyncAction::HandleCompleted));
		}
	}

}

void ULoadGltfFromDataAsyncAction::HandleCompleted(UglTFRuntimeAsset* GltfAsset) const
{
	OnCompleted.Broadcast(GltfAsset);
}
