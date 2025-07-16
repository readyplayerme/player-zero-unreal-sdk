// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RpmFunctionLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAssetIdFetched, FString, AssetId);

/**
 * 
 */
UCLASS()
class RPMNEXTGEN_API URpmFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

};
