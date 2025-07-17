// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
<<<<<<< HEAD:Source/PlayerZero/Public/PlayerZeroLoaderComponent.h
#include "Api/Assets/Models/Asset.h"
#include "Api/Characters/Models/PlayerZeroCharacter.h"
=======
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Characters/Models/RpmCharacter.h"
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmLoaderComponent.h
#include "Components/ActorComponent.h"
#include "PlayerZeroCharacterTypes.h"
#include "PlayerZeroLoaderComponent.generated.h"

class FAssetApi;
class FFileApi;
class FGlbLoader;
struct FCharacterFindByIdResponse;
class FCharacterApi;
struct FRpmAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFound, FRpmCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAssetLoaded, const FRpmCharacterData&, characterData, UglTFRuntimeAsset*, GltfRuntimeAsset );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERZERO_API UPlayerZeroLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Ready Player Me" )
	FOnCharacterFound OnCharacterFound;
	UPROPERTY(BlueprintAssignable, Category = "Ready Player Me" )
	FOnCharacterAssetLoaded OnCharacterAssetLoaded;
	URpmLoaderComponent();
	
	void SetGltfConfig(const FglTFRuntimeConfig* Config);
	
	void HandleCharacterAssetLoaded(const TArray<uint8>& Array, const FString& FileName);


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	FglTFRuntimeConfig GltfConfig;
	FString AppId;
	FPlayerZeroCharacter Character;
	FPlayerZeroCharacterData CharacterData;
	
	virtual void BeginPlay() override;
	
	virtual void FindCharacterById(const FString CharacterId);
>>>>>>> origin/develop:Source/RpmNextGen/Public/RpmLoaderComponent.h

	UFUNCTION(BlueprintCallable, Category = "Player Zero")
	virtual void LoadCharacterFromUrl(FString Url);
	
	virtual void HandleCharacterFindResponse(TSharedPtr<FCharacterFindByIdResponse> CharacterFindByIdResponse, bool bWasSuccessful);
	
private:
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
};
