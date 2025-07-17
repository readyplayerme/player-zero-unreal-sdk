#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"
#include "Models/RpmAsset.h"

struct FAssetLoadingContext;
struct FCachedAssetData;
class IHttpResponse;
class FHttpModule;
struct FRpmAsset;

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Assets/AssetIconLoader.h
class PLAYERZERO_API FAssetIconLoader : public TSharedFromThis<FAssetIconLoader>
=======
class RPMNEXTGEN_API FAssetIconLoader : public FWebApi
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Assets/AssetIconLoader.h
{
public:
	DECLARE_DELEGATE_TwoParams(FOnIconLoaded, const FRpmAsset&, const TArray<uint8>&);
	
	FOnIconLoaded OnIconLoaded;
	
	FAssetIconLoader();
	virtual ~FAssetIconLoader();
	
	void LoadIcon(const FRpmAsset& Asset);

private:
	FHttpModule* Http;
};
