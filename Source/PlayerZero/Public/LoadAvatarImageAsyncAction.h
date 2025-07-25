// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LoadAvatarImageAsyncAction.generated.h"

class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarImageLoaded, UTexture2D*, Texture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAvatarImageLoadFailed);

/**
 * 
 */
UCLASS()
class PLAYERZERO_API ULoadAvatarImageAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAvatarImageLoaded OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnAvatarImageLoadFailed OnFailed;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerZero|Avatar", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static ULoadAvatarImageAsyncAction* LoadAvatarImageAsync(UObject* WorldContextObject, const FString& AvatarId);

	virtual void Activate() override;

private:
	FString CachedAvatarId;
	UObject* ContextObject;

	void OnIconLoaded(UTexture2D* Texture);
};
