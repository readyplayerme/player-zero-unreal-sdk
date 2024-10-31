// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UMaterialInterface;
class UTexture;

class RPMNEXTGENEDITOR_API FAssetNameGenerator : public TSharedFromThis<FAssetNameGenerator>
{
public:
	FAssetNameGenerator();
	FAssetNameGenerator(FString InPath);
	virtual ~FAssetNameGenerator();

	void SetPath(const FString& NewPath);
	FString GetPath() const;

	virtual FString GenerateMaterialName(UMaterialInterface* Material, int32 MaterialIndex, const FString& SlotName) const;
	virtual FString GenerateTextureName(UTexture* Texture, UMaterialInterface* Material, const FString& MaterialPath, const FString& ParamName) const;

protected:
	FString Path;
};