#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"
#include "Models/Asset.h"

struct FAssetLoadingContext;
struct FCachedAssetData;
class IHttpResponse;
class FHttpModule;
struct FAsset;

DECLARE_DELEGATE_TwoParams(FOnGlbLoaded, const FAsset&, const TArray<uint8>&);
class RPMNEXTGEN_API FAssetGlbLoader : public FWebApi
{
public:
	FOnGlbLoaded OnGlbLoaded;
	FAssetGlbLoader();
	virtual ~FAssetGlbLoader();
	void LoadGlb(const FAsset& Asset, const FString& BaseModelId, bool bStoreInCache);
    
private:
	FHttpModule* Http;
};
