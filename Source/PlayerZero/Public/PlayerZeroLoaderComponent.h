// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
#include "Api/Characters/Models/PlayerZeroCharacter.h"
#include "Components/ActorComponent.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroLoaderComponent.generated.h"

struct FFileData;
class FFileApi;
class FGlbLoader;
struct FCharacterCreateResponse;
struct FCharacterUpdateResponse;
struct FCharacterFindByIdResponse;
class FCharacterApi;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFound, FPlayerZeroCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAssetLoaded, const FPlayerZeroCharacterData&, CharacterData, UglTFRuntimeAsset*, GltfRuntimeAsset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERZERO_API UPlayerZeroLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnCharacterAssetLoaded OnCharacterAssetLoaded;
	UPROPERTY(BlueprintAssignable, Category = "Player Zero" )
	FOnCharacterFound OnCharacterFound;

	UPlayerZeroLoaderComponent();
	
	void SetGltfConfig(const FglTFRuntimeConfig* Config);

	void HandleCharacterAssetLoaded(const FFileData& File, const TArray<unsigned char>& Array);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	FglTFRuntimeConfig GltfConfig;
	FString AppId;
	FPlayerZeroCharacter Character;
	FPlayerZeroCharacterData CharacterData;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacterFromUrl(FString Url);

	UFUNCTION()
	virtual void HandleCharacterFindResponse(FCharacterFindByIdResponse CharacterFindByIdResponse, bool bWasSuccessful);
	
private:
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
};
