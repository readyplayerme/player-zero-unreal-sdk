#pragma once

#include "CharacterBlueprint.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterBlueprintModel
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "name"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "name"))
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "characterModel"))
	FDateTime CreatedAt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "characterModel"))
	FDateTime UpdatedAt;
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FCharacterBlueprint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "name"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "name"))
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "modelUrl"))
	FString ModelUrl;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "iconUrl"))
	FString IconUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "characterModel"))
	FCharacterBlueprintModel CharacterModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "characterModel"))
	FDateTime CreatedAt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me", meta = (JsonName = "characterModel"))
	FDateTime UpdatedAt;
};



