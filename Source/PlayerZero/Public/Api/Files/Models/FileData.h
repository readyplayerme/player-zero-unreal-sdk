#pragma once

#include "CoreMinimal.h"
#include "FileData.generated.h"

UENUM(BlueprintType)
enum class EFileType : uint8
{
	None,
	GLB,
	PNG,
	JPEG,
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FFileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "name"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "Url"))
	FString Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "fileType"))
	EFileType FileType = EFileType::None;

	FFileData()
	{
		Name = "";
		Url = "";
	}
	
	FFileData(const FString& InUrl, const FString& InName = "")
		: Name(InName), Url(InUrl)
	{
		UE_LOG(LogTemp, Warning, TEXT("FFileData In URL: %s"), *InUrl);
		// Step 1: Remove query parameters for processing
		FString CleanUrl = InUrl;
		int32 QueryStartIndex;
		if (InUrl.FindChar(TEXT('?'), QueryStartIndex))
		{
			CleanUrl = InUrl.Left(QueryStartIndex);
		}
		UE_LOG(LogTemp, Warning, TEXT("CleanUrl: %s"), *CleanUrl);
		// Step 2: Determine file type from extension (case-insensitive)
		if (CleanUrl.EndsWith(TEXT(".glb"), ESearchCase::IgnoreCase))
		{
			FileType = EFileType::GLB;
		}
		else if (CleanUrl.EndsWith(TEXT(".png"), ESearchCase::IgnoreCase))
		{
			FileType = EFileType::PNG;
		}
		else if (CleanUrl.EndsWith(TEXT(".jpg"), ESearchCase::IgnoreCase) ||
				 CleanUrl.EndsWith(TEXT(".jpeg"), ESearchCase::IgnoreCase))
		{
			FileType = EFileType::JPEG;
		}
		else
		{
			FileType = EFileType::None;
			UE_LOG(LogTemp, Error, TEXT("Unable to determine file type for URL: %s"), *InUrl);
		}

		// Step 3: Fallback to filename (no query) if Name was not provided
		if (Name.IsEmpty())
		{
			Name = FPaths::GetCleanFilename(CleanUrl); // Use clean URL for filename
		}
	}
};