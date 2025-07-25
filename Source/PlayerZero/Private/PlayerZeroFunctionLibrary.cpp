// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroFunctionLibrary.h"

#include "PlayerZero.h"
#include "PlayerZeroSubsystem.h"
#include "Api/Characters/Models/PlayerZeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/PlayerZeroImageHelper.h"

FString UPlayerZeroFunctionLibrary::GetHotLoadedAvatarId(UObject* WorldContextObject)
{
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		if (UPlayerZeroSubsystem* Subsystem = GameInstance->GetSubsystem<UPlayerZeroSubsystem>())
		{
			return Subsystem->GetHotLoadedAvatarId();
		}
	}
	return FString();
}
