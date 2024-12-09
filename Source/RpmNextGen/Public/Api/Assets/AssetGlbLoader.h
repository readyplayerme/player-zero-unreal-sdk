#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"
#include "Models/RpmAsset.h"

struct FAssetLoadingContext;
struct FCachedAssetData;
class IHttpResponse;
class FHttpModule;
struct FRpmAsset;

DECLARE_DELEGATE_TwoParams(FOnGlbLoaded, const FRpmAsset&, const TArray<uint8>&);
class RPMNEXTGEN_API FAssetGlbLoader : public FWebApi
{
public:
	FOnGlbLoaded OnGlbLoaded;
	FAssetGlbLoader();
	virtual ~FAssetGlbLoader();
	void LoadGlb(const FRpmAsset& Asset, const FString& CharacterStyleId, bool bStoreInCache);
    
private:
	FHttpModule* Http;
};
