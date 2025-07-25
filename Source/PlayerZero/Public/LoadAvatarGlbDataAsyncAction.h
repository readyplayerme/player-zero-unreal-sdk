// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LoadAvatarGlbDataAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarDataDownloadedDelegate, const TArray<uint8>&, Data);

UCLASS()
class PLAYERZERO_API ULoadAvatarGlbDataAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAvatarDataDownloadedDelegate OnCompleted;

	UFUNCTION(BlueprintCallable, Category = "PlayerZero|Avatar", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static ULoadAvatarGlbDataAsyncAction* Download(UObject* WorldContextObject, const FString& Url);

	virtual void Activate() override;

private:
	FString CachedUrl;
	UObject* ContextObject;

	void OnDataDownloaded(const TArray<uint8>& Data);
};