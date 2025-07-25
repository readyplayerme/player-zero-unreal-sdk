// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LoadAvatarImageAsyncAction.generated.h"

class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarIconTextureLoaded, UTexture2D*, Texture);

/**
 * 
 */
UCLASS()
class PLAYERZERO_API ULoadAvatarImageAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAvatarIconTextureLoaded OnCompleted;

	UFUNCTION(BlueprintCallable, Category = "PlayerZero|Avatar", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static ULoadAvatarImageAsyncAction* LoadAvatarImageAsync(UObject* WorldContextObject, const FString& AvatarId);

	virtual void Activate() override;

private:
	FString CachedAvatarId;
	UObject* ContextObject;

	void OnIconLoaded(UTexture2D* Texture);
};
