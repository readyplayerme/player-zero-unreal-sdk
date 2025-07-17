#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"
#include "Models/RpmAsset.h"

struct FAssetLoadingContext;
struct FCachedAssetData;
class IHttpResponse;
class FHttpModule;
struct FRpmAsset;

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Assets/AssetGlbLoader.h
class PLAYERZERO_API FAssetGlbLoader : public TSharedFromThis<FAssetGlbLoader>
=======
DECLARE_DELEGATE_TwoParams(FOnGlbLoaded, const FRpmAsset&, const TArray<uint8>&);
class RPMNEXTGEN_API FAssetGlbLoader : public FWebApi
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Assets/AssetGlbLoader.h
{
public:
	FOnGlbLoaded OnGlbLoaded;
	FAssetGlbLoader();
	virtual ~FAssetGlbLoader();
	void LoadGlb(const FRpmAsset& Asset, const FString& CharacterStyleId);
    
private:
	FHttpModule* Http;
};
