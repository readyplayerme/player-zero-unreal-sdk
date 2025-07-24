// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZeroFunctionLibrary.h"

#include "PlayerZero.h"
#include "PlayerZeroSubsystem.h"
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

void UPlayerZeroFunctionLibrary::LoadAvatarImage(UObject* WorldContextObject, const FString& AvatarId, const FVector2D& ImageSize, UImage* Image)
{
	if (!WorldContextObject || !Image)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Invalid context or image."));
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Could not get UWorld from context."));
		return;
	}

	if (UPlayerZeroSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UPlayerZeroSubsystem>())
	{
		Subsystem->GetAvatarIconAsTexture(AvatarId, FOnAvatarTextureLoaded::CreateLambda(
			[Image, ImageSize](UTexture2D* Texture)
			{
				if (Texture)
				{
					FPlayerZeroImageHelper::LoadTextureToUImage(Texture, ImageSize, Image);
				}
				else
				{
					UE_LOG(LogPlayerZero, Error, TEXT("GetAvatarImage: Failed to load avatar icon texture."));
				}
			}));
	}
	else
	{
		UE_LOG(LogPlayerZero, Error, TEXT("GetAvatarImage: PlayerZeroSubsystem not found."));
	}
}
