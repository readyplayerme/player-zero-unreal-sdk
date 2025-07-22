// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroFunctionLibrary.h"

#include "PlayerZeroSubsystem.h"

FString UPlayerZeroFunctionLibrary::GetHotLoadedAvatarId()
{
	if (UPlayerZeroSubsystem* Subsystem = GEngine->GetEngineSubsystem<UPlayerZeroSubsystem>())
	{
		return Subsystem->GetHotLoadedAvatarId();
	}
	return FString();
}
