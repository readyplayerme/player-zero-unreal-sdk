// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerZeroFunctionLibrary.generated.h"

class UImage;
class UPlayerZeroSubsystem;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAvatarImageLoaded, const UImage*, AvatarImage);
/**
 * 
 */
UCLASS()
class PLAYERZERO_API UPlayerZeroFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerZero")
	static FString GetHotLoadedAvatarId(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "PlayerZero|Avatar", meta = (WorldContext = "WorldContextObject"))
	static void LoadAvatarImage(UObject* WorldContextObject, const FString& AvatarId, const FVector2D& ImageSize, UImage* Image);

};
