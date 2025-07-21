#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerZeroCharacterTypes.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FPlayerZeroAnimationConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero" )
	USkeleton* Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	FPlayerZeroAnimationConfig()
	{
		Skeleton = nullptr;
		AnimationBlueprint = nullptr;
	}
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FPlayerZeroCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FString BaseModelId;

	FPlayerZeroCharacterData()
	{
		Id = "";
		BaseModelId = "";
	}
};
