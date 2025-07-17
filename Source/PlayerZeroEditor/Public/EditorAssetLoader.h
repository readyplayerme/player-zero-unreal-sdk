#pragma once

#include "CoreMinimal.h"
#include "Api/Assets/AssetGlbLoader.h"
#include "HAL/PlatformFilemanager.h"

struct FglTFRuntimeConfig;
class UglTFRuntimeAsset;

class PLAYERZEROEDITOR_API FEditorAssetLoader : public FAssetGlbLoader
{
public:
	USkeleton* SkeletonToCopy;

	FEditorAssetLoader();
	virtual ~FEditorAssetLoader() override;

	void LoadAssetToWorldAsUPlayerZeroActor(UglTFRuntimeAsset* GltfAsset, FString AssetId = "");
	void LoadBaseModelAsset(const FAsset& Asset);

private:
	FglTFRuntimeConfig* GltfConfig;
	
	void LoadAssetToWorld(const FString& AssetId, UglTFRuntimeAsset* GltfAsset);
	UFUNCTION()
	void HandleGlbLoaded(const FAsset& Asset, const TArray<unsigned char>& Data);
};
