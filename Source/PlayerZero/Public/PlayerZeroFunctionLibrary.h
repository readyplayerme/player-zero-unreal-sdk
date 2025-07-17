// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayerZeroFunctionLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAssetIdFetched, FString, AssetId);

/**
 * 
 */
UCLASS()
class PLAYERZERO_API UPlayerZeroFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroFunctionLibrary.h
	UFUNCTION(BlueprintCallable, Category = "PlayerZero", meta = (WorldContext = "WorldContextObject"))
	static void FetchFirstAssetId(UObject* WorldContextObject, const FString& AssetType, FOnAssetIdFetched OnAssetIdFetched);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerZero/Cache")
=======
	UFUNCTION(BlueprintCallable, Category = "ReadyPlayerMe/Cache")
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmFunctionLibrary.h
	static void ExtractCachePakFile();
};
