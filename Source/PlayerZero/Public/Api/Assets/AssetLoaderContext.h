#pragma once

#include "CoreMinimal.h"
#include "Models/RpmAsset.h"

struct FAssetLoadingContext
{
	FRpmAsset Asset;
	FString CharacterStyleId;
	TArray<uint8> Data;
	FAssetLoadingContext(const FRpmAsset& InAsset, const FString& InCharacterStyleId)
		: Asset(InAsset), CharacterStyleId(InCharacterStyleId)
	{
	}
};
