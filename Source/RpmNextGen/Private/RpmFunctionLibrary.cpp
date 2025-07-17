// Copyright Ready Player Me


#include "RpmFunctionLibrary.h"
#include "Api/Files/PakFileUtility.h"
#include "Cache/AssetCacheManager.h"


void URpmFunctionLibrary::ExtractCachePakFile()
{
    const FString PakFilePath = FFileUtility::GetFullPersistentPath(FPakFileUtility::CachePakFilePath);

	FPakFileUtility::ExtractFilesFromPak(PakFilePath);
}
