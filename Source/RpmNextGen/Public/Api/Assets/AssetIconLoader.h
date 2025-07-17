#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"
#include "Models/RpmAsset.h"

struct FAssetLoadingContext;
struct FCachedAssetData;
class IHttpResponse;
class FHttpModule;
struct FRpmAsset;

class RPMNEXTGEN_API FAssetIconLoader : public FWebApi
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
