﻿#pragma once

#include "CoreMinimal.h"
#include "DeveloperLoginResponse.h"
#include "DeveloperAuth.generated.h"

USTRUCT(BlueprintType)
struct PLAYERZEROEDITOR_API FDeveloperAuth
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero",  meta = (JsonName = "token"))
	FString Token;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "refreshToken"))
	FString RefreshToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "name"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Zero", meta = (JsonName = "isDemo"))
	bool IsDemo;

	FDeveloperAuth() = default;
	
	FDeveloperAuth(FDeveloperLoginResponseBody Data, bool bIsDemo)
	{
		Token = Data.Token;
		RefreshToken = Data.RefreshToken;
		Name = Data.Name;
		IsDemo = bIsDemo;
	}

	FString ToJsonString() const
	{
		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		Writer->WriteObjectStart();
		Writer->WriteValue(TEXT("token"), Token);
		Writer->WriteValue(TEXT("refreshToken"), RefreshToken);
		Writer->WriteValue(TEXT("name"), Name);
		Writer->WriteValue(TEXT("isDemo"), IsDemo);
		Writer->WriteObjectEnd();
		Writer->Close();
		return OutputString;
	}

	static bool FromJsonString(const FString& JsonString, FDeveloperAuth& OutStruct)
	{
		return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutStruct, 0, 0);
	}

	bool IsValid() const
	{
		return IsDemo || !Token.IsEmpty();
	}
};
