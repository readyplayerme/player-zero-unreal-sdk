#pragma once

#include "CoreMinimal.h"
#include "Asset.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "id"))
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "name"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "glbUrl"))
	FString GlbUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "iconUrl"))
	FString IconUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "createdAt"))
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "createdAt"))
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "updatedAt"))
	FDateTime UpdatedAt;

	FAsset()
	{
		Id = "";
		Name = "";
		GlbUrl = "";
		IconUrl = "";
		Type = "";
		CreatedAt = FDateTime();
		UpdatedAt = FDateTime();
	}
	
private:
	UPROPERTY(meta = (JsonIgnore))
	FString organizationId = "";
	
	UPROPERTY(meta = (JsonIgnore))
	FString cageMeshId = "";
	
	UPROPERTY(meta = (JsonIgnore))
	bool active = false;
	
	UPROPERTY(meta = (JsonIgnore))
	FString _id = "";
};