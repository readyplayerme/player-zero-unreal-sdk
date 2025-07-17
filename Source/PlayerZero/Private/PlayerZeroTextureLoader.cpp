#include "PlayerZeroTextureLoader.h"

#include "PlayerZero.h"
#include "Api/Assets/AssetIconLoader.h"
#include "Async/Async.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Utilities/PlayerZeroImageHelper.h"

FPlayerZeroTextureLoader::FPlayerZeroTextureLoader()
{
	AssetIconLoader = MakeShared<FAssetIconLoader>();
	AssetIconLoader->OnIconLoaded.BindRaw( this, &FPlayerZeroTextureLoader::OnIconLoaded);
}

void FPlayerZeroTextureLoader::LoadIconFromAsset(const FAsset& Asset, bool bStoreInCache)
{
	AssetIconLoader->LoadIcon(Asset, bStoreInCache);
}

void FPlayerZeroTextureLoader::OnIconLoaded(const FAsset& Asset, const TArray<unsigned char>& Array)
{
	if (UTexture2D* Texture = FPlayerZeroImageHelper::CreateTextureFromData(Array))
	{
		OnTextureLoaded.ExecuteIfBound(Texture);
		return;
	}
	UE_LOG(LogPlayerZero, Error, TEXT("Failed to load icon for asset: %s"), *Asset.Id);
	OnTextureLoaded.ExecuteIfBound(nullptr);
}
