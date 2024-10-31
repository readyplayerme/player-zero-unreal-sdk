// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetSaver/AssetNameGenerator.h"

FAssetNameGenerator::FAssetNameGenerator() : Path(TEXT(""))
{
}

FAssetNameGenerator::FAssetNameGenerator(FString InPath) : Path(InPath)
{
}

FAssetNameGenerator::~FAssetNameGenerator()
{
}

void FAssetNameGenerator::SetPath(const FString& NewPath)
{
	Path = NewPath;
}

FString FAssetNameGenerator::GetPath() const
{
	return Path;
}

FString FAssetNameGenerator::GenerateMaterialName(UMaterialInterface* Material, int32 MaterialIndex, const FString& SlotName) const
{
	return FString::Printf(TEXT("%sMaterial_%d"), *Path, MaterialIndex);
}

FString FAssetNameGenerator::GenerateTextureName(UTexture* Texture, UMaterialInterface* Material, const FString& MaterialPath, const FString& ParamName) const
{
	return FString::Printf(TEXT("%s%s%s"), *Path, *Material->GetName(), *ParamName);
}