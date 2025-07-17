#pragma once

#include "CoreMinimal.h"
#include "RefreshTokenRequest.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZERO_API FRefreshTokenRequestBody
{
	GENERATED_BODY()

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Auth/Models/RefreshTokenRequest.h
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me")
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Auth/Models/RefreshTokenRequest.h
	FString Token;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
	FString RefreshToken;
};

USTRUCT(BlueprintType)
struct PLAYERZERO_API FRefreshTokenRequest
{
	GENERATED_BODY()

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Auth/Models/RefreshTokenRequest.h

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero")
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ready Player Me")
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Auth/Models/RefreshTokenRequest.h
	FRefreshTokenRequestBody Data;
};
