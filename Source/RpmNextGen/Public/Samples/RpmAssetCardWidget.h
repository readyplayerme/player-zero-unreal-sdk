// Copyright Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Blueprint/UserWidget.h"
#include "RpmAssetCardWidget.generated.h"

class FRpmTextureLoader;
class UImage;
class UTextBlock;

UCLASS()
class RPMNEXTGEN_API URpmAssetCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AssetCategoryText;
	
	UPROPERTY(meta = (BindWidget))
	UImage* AssetImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AssetNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AssetIdText;
	
	UFUNCTION()
	void OnTextureLoaded(UTexture2D* Texture2D);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	virtual void InitializeCard(const FRpmAsset& Asset);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void LoadImage(const FRpmAsset& Asset);

	virtual void NativeConstruct() override;
	
private:
	FRpmAsset AssetData;
	TSharedPtr<FRpmTextureLoader> TextureLoader;
};
