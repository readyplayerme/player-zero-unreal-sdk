#pragma once
#include "CoreMinimal.h"

class FAssetNameGenerator;
class FString;
class UMaterialInterface;
class UStaticMesh;
class USkeletalMesh;
class USkeleton;
class UTexture;

class RPMNEXTGENEDITOR_API FRpmAssetSaver
{
public:
    static bool SaveSkeletonAsset(USkeleton* Skeleton, const FString& Path);
    static bool SaveSkeletalMeshAsset(USkeletalMesh* SkeletalMesh, const FString& SkeletalMeshPath, const FString& SkeletonPath, const FString& PhysicsAssetPath, TSharedPtr<FAssetNameGenerator> AssetNameGenerator = nullptr);
    static bool SaveStaticMeshAsset(UStaticMesh* StaticMesh, const FString& StaticMeshPath, TSharedPtr<FAssetNameGenerator> AssetNameGenerator = nullptr);
    static bool SaveMaterialAsset(UMaterialInterface* Material, const FString& MaterialPath, UMaterialInterface*& OutMaterial, TSharedPtr<FAssetNameGenerator> AssetNameGenerator = nullptr);

private:
    static bool IsTransient(UObject* Object);
    static bool SaveUObject(UObject* Object, const FString& Path);
};
