#pragma once

#include "CoreMinimal.h"
#include "Models/Asset.h"

struct FAssetLoadingContext
{
	FAsset Asset;
	FString CharacterStyleId;
	TArray<uint8> Data;
	bool bStoreInCache;
	FAssetLoadingContext(const FAsset& InAsset, const FString& InCharacterStyleId, bool bInStoreInCache)
		: Asset(InAsset), CharacterStyleId(InCharacterStyleId), bStoreInCache(bInStoreInCache)
	{
	}
};
