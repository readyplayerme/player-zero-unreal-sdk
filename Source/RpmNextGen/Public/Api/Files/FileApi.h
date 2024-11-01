#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

struct FRpmAsset;
DECLARE_DELEGATE_TwoParams(FOnFileRequestComplete, const TArray<uint8>&, const FString&);
DECLARE_DELEGATE_TwoParams(FOnAssetFileRequestComplete, const TArray<uint8>&, const FRpmAsset&);

class RPMNEXTGEN_API FFileApi : public TSharedFromThis<FFileApi>
{
public:
	FOnAssetFileRequestComplete OnAssetFileRequestComplete;
	FOnFileRequestComplete OnFileRequestComplete;
	
	FFileApi();
	virtual ~FFileApi();
	
	virtual void LoadFileFromUrl(const FString& URL);
	virtual void LoadAssetFileFromUrl(const FString& URL, const FRpmAsset& Asset);
	virtual void FileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	virtual void AssetFileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FRpmAsset Asset);
	static bool LoadFileFromPath(const FString& Path, TArray<uint8>& OutContent);
};
