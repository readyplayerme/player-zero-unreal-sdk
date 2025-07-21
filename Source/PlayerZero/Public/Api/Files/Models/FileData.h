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
	
	FFileData(const FString& InUrl , const FString& InName = "")
		: Name(InName), Url(InUrl)
	{
		if (InUrl.EndsWith(TEXT(".glb")))
		{
			FileType = EFileType::GLB;
		}
		else if (InUrl.EndsWith(TEXT(".png")))
		{
			FileType = EFileType::PNG;
		}
		else if (InUrl.EndsWith(TEXT(".jpg")) || InUrl.EndsWith(TEXT(".jpeg")))
		{
			FileType = EFileType::JPEG;
		}
		else
		{
			FileType = EFileType::None;
			UE_LOG(LogTemp, Error, TEXT("Unable to determine file type for URL: %s"), *InUrl);
		}

		if (Name.IsEmpty())
		{
			Name = FPaths::GetCleanFilename(InUrl);
		}
	}
};