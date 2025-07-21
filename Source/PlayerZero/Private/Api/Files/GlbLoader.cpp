#include "Api/Files/GlbLoader.h"
#include "PlayerZero.h"
#include "glTFRuntime/Public/glTFRuntimeFunctionLibrary.h"
#include "Api/Files//FileUtility.h"

FGlbLoader::FGlbLoader() : GltfConfig(new FglTFRuntimeConfig()), FileWriter(new FFileUtility()) 
{
    GltfConfig->TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
    OnAssetFileRequestComplete.BindRaw( this, &FGlbLoader::HandleFileRequestComplete);
}

FGlbLoader::FGlbLoader(FglTFRuntimeConfig* Config) : FGlbLoader()
{
    if (Config)
    {
        delete GltfConfig;
        GltfConfig = Config;
    }
}

FGlbLoader::~FGlbLoader()
{
    delete GltfConfig;  
    delete FileWriter;
}

void FGlbLoader::HandleFileRequestComplete(const FFileData& File, const TArray<unsigned char>& Data)
{
    UglTFRuntimeAsset* GltfAsset = nullptr;
    if (Data.Num() < 1)
    {
        if(OnGLtfAssetLoaded.IsBound())
        {
            GltfAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, *GltfConfig);
            OnGLtfAssetLoaded.ExecuteIfBound(GltfAsset, TEXT("AssetType"));
        }
        return;
    }
    UE_LOG(LogPlayerZero, Error, TEXT("Failed to load GLB from URL"));
    OnGLtfAssetLoaded.ExecuteIfBound(GltfAsset, TEXT(""));
}
