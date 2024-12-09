// Copyright Ready Player Me

#include "Samples/RpmAssetButtonWidget.h"
#include "RpmTextureLoader.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Utilities/RpmImageHelper.h"

void URpmAssetButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (AssetButton)
    {
        AssetButton->OnClicked.AddDynamic(this, &URpmAssetButtonWidget::HandleButtonClicked);
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
        DefaultColor = AssetButton->GetBackgroundColor();
#else
        DefaultColor = AssetButton->BackgroundColor;
#endif

    }
    if(!TextureLoader.IsValid())
    {
        TextureLoader = MakeShared<FRpmTextureLoader>();
        TextureLoader->OnTextureLoaded.BindUObject(this, &URpmAssetButtonWidget::OnTextureLoaded);
    }
}

void URpmAssetButtonWidget::InitializeButton(const FRpmAsset& InAssetData, const FVector2D& InImageSize)
{    
    AssetData = InAssetData;

    if (AssetImage)
    {
        AssetImage->SetDesiredSizeOverride(InImageSize);
        TextureLoader->LoadIconFromAsset(AssetData);
    }
}

void URpmAssetButtonWidget::HandleButtonClicked() 
{
    SetSelected(!bIsSelected);
    OnAssetButtonClicked.Broadcast(this);
}

void URpmAssetButtonWidget::SetSelected(const bool bInIsSelected)
{
    bIsSelected = bInIsSelected;

    if (AssetButton)
    {
        const FLinearColor NewColor = bInIsSelected ? SelectedColor : DefaultColor;
        AssetButton->SetBackgroundColor(NewColor);
    }
}

void URpmAssetButtonWidget::OnTextureLoaded(UTexture2D* Texture2D)
{
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
    FRpmImageHelper::LoadTextureToUImage(Texture2D, AssetImage->GetBrush().ImageSize, AssetImage);
#else
    FRpmImageHelper::LoadTextureToUImage(Texture2D, AssetImage->Brush.ImageSize, AssetImage);
#endif
}
