// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAsset.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Api/Characters/Models/RpmCharacter.h"
#include "Components/ActorComponent.h"
#include "RpmCharacterTypes.h"
#include "RpmLoaderComponent.generated.h"

class FAssetApi;
class FFileApi;
class FGlbLoader;
struct FCharacterFindByIdResponse;
class FCharacterApi;
struct FRpmAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFound, FRpmCharacterData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAssetLoaded, const FRpmCharacterData&, characterData, UglTFRuntimeAsset*, GltfRuntimeAsset );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPMNEXTGEN_API URpmLoaderComponent : public UActorComponent
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
	FRpmCharacter Character;
	FRpmCharacterData CharacterData;
	
	virtual void BeginPlay() override;
	
	virtual void FindCharacterById(const FString CharacterId);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void LoadCharacterFromUrl(FString Url);
	
	virtual void HandleCharacterFindResponse(TSharedPtr<FCharacterFindByIdResponse> CharacterFindByIdResponse, bool bWasSuccessful);
	
private:
	TSharedPtr<FCharacterApi> CharacterApi;
	TSharedPtr<FFileApi> FileApi;
};
