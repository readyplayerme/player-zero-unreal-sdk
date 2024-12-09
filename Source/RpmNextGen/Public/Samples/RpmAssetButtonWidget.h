// Copyright Ready Player Me
#pragma once

#include "CoreMinimal.h"
#include "Api/Assets/Models/RpmAsset.h"
#include "Blueprint/UserWidget.h"
#include "RpmAssetButtonWidget.generated.h"

class FRpmTextureLoader;
class USizeBox;
class UBorder;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetButtonClicked, const URpmAssetButtonWidget*, AssetData);

/**
 * 
 */
UCLASS()
class RPMNEXTGEN_API URpmAssetButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* AssetButton;

	UPROPERTY(meta = (BindWidget))
	UImage* AssetImage;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAssetButtonClicked OnAssetButtonClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Button" )
	FLinearColor SelectedColor = FLinearColor::Yellow;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me|Asset Button")
	virtual void InitializeButton(const FRpmAsset& InAssetData, const FVector2D& InImageSize);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me|Asset Button")
	virtual void SetSelected(const bool bInIsSelected);

	FRpmAsset GetAssetData() const { return AssetData; }
	
protected:
	UFUNCTION()
	void OnTextureLoaded(UTexture2D* Texture2D);
	
	virtual void NativeConstruct() override;

private:
	TSharedPtr<FRpmTextureLoader> TextureLoader;
	FLinearColor DefaultColor;
	FRpmAsset AssetData;
	bool bIsSelected;
	
	UFUNCTION()
	virtual void HandleButtonClicked();
};
