#pragma once

#include "CoreMinimal.h"
#include "Api/Assets/AssetGlbLoader.h"
#include "HAL/PlatformFilemanager.h"

struct FglTFRuntimeConfig;
class UglTFRuntimeAsset;

class RPMNEXTGENEDITOR_API FEditorAssetLoader : public FAssetGlbLoader
{
public:
	USkeleton* SkeletonToCopy;

	FEditorAssetLoader();
	virtual ~FEditorAssetLoader() override;

	void LoadAssetToWorldAsURpmActor(UglTFRuntimeAsset* GltfAsset, FString AssetId = "");
	void LoadBCharacterStyleAsset(const FRpmAsset& Asset);
	
	USkeletalMesh* SaveAsUAsset(UglTFRuntimeAsset* GltfAsset, const FString& LoadedAssetId) const;

private:
	FglTFRuntimeConfig* GltfConfig;
	
	void LoadAssetToWorld(const FString& AssetId, UglTFRuntimeAsset* GltfAsset);
	UFUNCTION()
	void HandleGlbLoaded(const FRpmAsset& Asset, const TArray<unsigned char>& Data);
};
