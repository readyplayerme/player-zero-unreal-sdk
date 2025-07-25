// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Api/Characters/Models/PlayerZeroCharacter.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LoadAvatarMetaDataAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvatarMetaDataLoadedBP, const FPlayerZeroCharacter&, MetaData);

UCLASS()
class PLAYERZERO_API ULoadAvatarMetaDataAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAvatarMetaDataLoadedBP OnCompleted;

	UFUNCTION(BlueprintCallable, Category = "PlayerZero|Avatar", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static ULoadAvatarMetaDataAsyncAction* Get(UObject* WorldContextObject, const FString& AvatarId);

	virtual void Activate() override;

private:
	FString CachedAvatarId;
	UObject* ContextObject;

	void OnDataLoaded(const FPlayerZeroCharacter& Character);
};
