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

<<<<<<< HEAD:Source/PlayerZeroEditor/Public/EditorAssetLoader.h
	void LoadAssetToWorldAsUPlayerZeroActor(UglTFRuntimeAsset* GltfAsset, FString AssetId = "");
	void LoadBaseModelAsset(const FAsset& Asset);
=======
	void LoadAssetToWorldAsURpmActor(UglTFRuntimeAsset* GltfAsset, FString AssetId = "");
	void LoadBCharacterStyleAsset(const FRpmAsset& Asset);

private:
	FglTFRuntimeConfig* GltfConfig;
	
	void LoadAssetToWorld(const FString& AssetId, UglTFRuntimeAsset* GltfAsset);
	UFUNCTION()
	void HandleGlbLoaded(const FRpmAsset& Asset, const TArray<unsigned char>& Data);
};
