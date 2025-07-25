// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LoadAvatarGltfAssetAsyncAction.generated.h"

class UglTFRuntimeAsset;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarGltfLoaded, UglTFRuntimeAsset*, GltfAsset);

/**
 * 
 */
UCLASS()
class PLAYERZERO_API ULoadAvatarGltfAssetAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAvatarGltfLoaded OnCompleted;

	UFUNCTION(BlueprintCallable, Category = "PlayerZero|Avatar", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static ULoadAvatarGltfAssetAsyncAction* LoadAvatarGltfAssetAsync(UObject* WorldContextObject, const FString& AvatarId);

	virtual void Activate() override;
	void HandleCompleted(UglTFRuntimeAsset* GltfAsset) const;

private:
	FString CachedAvatarId;
	UObject* ContextObject;
	
};
