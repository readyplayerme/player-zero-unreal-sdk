#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

struct FFileData;
struct FAsset;
DECLARE_DELEGATE_TwoParams(FOnFileRequestComplete, const TArray<uint8>*, const FString&);
DECLARE_DELEGATE_TwoParams(FOnAssetFileRequestComplete, const TArray<uint8>*, const FAsset&);

class PLAYERZERO_API FFileApi : public TSharedFromThis<FFileApi>
{
public:
	FOnAssetFileRequestComplete OnAssetFileRequestComplete;
	FOnFileRequestComplete OnFileRequestComplete;
	
	FFileApi();
	virtual ~FFileApi();
	virtual void LoadFileFromUrl(const FString& URL);
	virtual void LoadAssetFileFromUrl(const FString& URL, FFileData File);
	virtual void FileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	virtual void AssetFileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FFileData File);
	static bool LoadFileFromPath(const FString& Path, TArray<uint8>& OutContent);
};
