#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

struct FFileData;
DECLARE_DELEGATE_TwoParams(FOnAssetFileRequestComplete, const FFileData&, const TArray<uint8>&);

class PLAYERZERO_API FFileApi : public TSharedFromThis<FFileApi>
{
public:
	FOnAssetFileRequestComplete OnAssetFileRequestComplete;
	
	FFileApi();
	virtual ~FFileApi();
	virtual void LoadFileFromUrl(const FString& URL);
	virtual void FileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FFileData File);
};
