#pragma once

#include "CoreMinimal.h"
#include "Models/RpmAsset.h"

struct FAssetLoadingContext
{
	FRpmAsset Asset;
	FString CharacterStyleId;
	TArray<uint8> Data;
	bool bStoreInCache;
	FAssetLoadingContext(const FRpmAsset& InAsset, const FString& InCharacterStyleId, bool bInStoreInCache)
		: Asset(InAsset), CharacterStyleId(InCharacterStyleId), bStoreInCache(bInStoreInCache)
	{
	}
};
