// Fill out your copyright notice in the Description page of Project Settings.

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

};
