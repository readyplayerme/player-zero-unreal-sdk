#include "AssetSaver/RpmAssetSaver.h"
#include "IMeshBuilderModule.h"
#include "Rendering/SkeletalMeshLODImporterData.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Materials/MaterialInstanceConstant.h"
#include "LODUtilities.h"
#include "RpmNextGen.h"
#include "AssetSaver/AssetNameGenerator.h"
#include "Materials/MaterialInstanceDynamic.h"

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
#include "Engine/SkinnedAssetCommon.h"
#endif

bool FRpmAssetSaver::IsTransient(UObject* Object)
{
	while (Object)
	{
		if (Object->HasAnyFlags(RF_Transient) || Object->IsA<UMaterialInstanceDynamic>())
		{
			return true;
		}
		Object = Object->GetOuter();
	}
	return false;
}

bool FRpmAssetSaver::SaveUObject(UObject* Object, const FString& Path)
{
	if (!FPackageName::IsValidObjectPath(Path))
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Invalid UPackage path %s"), *Path);
		return false;
	}

	if (FindPackage(nullptr, *Path) || LoadPackage(nullptr, *Path, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone))
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("UPackage %s already exists"), *Path);
		return false;
	}

	UPackage* NewPackage = CreatePackage(*Path);
	if (!NewPackage)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Unable to create UPackage %s"), *Path);
		return false;
	}

	UPackage* Package = Object->GetPackage();
	if (!Object->Rename(nullptr, NewPackage, REN_DontCreateRedirectors))
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Unable to move UObject %s into UPackage %s"), *Object->GetFullName(), *Path);
		return false;
	}

	if (Package)
	{
		Package->ClearDirtyFlag();
	}

	Object->SetFlags(Object->GetFlags() | RF_Public | RF_Standalone);

	FString NewName = FPackageName::GetShortName(Path);
	if (!Object->Rename(*NewName, nullptr, REN_DontCreateRedirectors))
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Unable to rename UObject %s to %s"), *Object->GetFullName(), *NewName);
		return false;
	}

	if (!UPackage::SavePackage(NewPackage, Object, EObjectFlags::RF_Standalone | EObjectFlags::RF_Public, *FPackageName::LongPackageNameToFilename(Path, FPackageName::GetAssetPackageExtension())))
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Unable to save UPackage %s"), *NewPackage->GetPathName());
		return false;
	}

	return true;
}

bool FRpmAssetSaver::SaveSkeletonAsset(USkeleton* Skeleton, const FString& Path)
{
	if (!Skeleton)
	{
		return false;
	}

	return SaveUObject(Skeleton, Path);
}

bool FRpmAssetSaver::SaveMaterialAsset(UMaterialInterface* Material, const FString& MaterialPath, UMaterialInterface*& OutMaterial, TSharedPtr<FAssetNameGenerator> AssetNameGenerator)
{
	// If no generator is provided, create a default instance
	if (!AssetNameGenerator.IsValid())
	{
		AssetNameGenerator = MakeShared<FAssetNameGenerator>();
	}
	
	if (!Material)
	{
		return false;
	}

	UE_LOG(LogReadyPlayerMe, Error, TEXT("Material %s %s %d"), *(Material->GetFullName()), *(Material->GetOutermost()->GetFullName()), Material->GetOutermost()->HasAnyFlags(EObjectFlags::RF_Transient));
	UMaterialInstanceDynamic* MaterialInstanceDynamic = Cast<UMaterialInstanceDynamic>(Material);
	if (MaterialInstanceDynamic)
	{
		UMaterialInstanceConstant* MaterialInstance = NewObject<UMaterialInstanceConstant>();
		MaterialInstance->SetParentEditorOnly(MaterialInstanceDynamic->Parent);
		TArray<FMaterialParameterInfo> MaterialsParameterInfos;
		TArray<FGuid> ParameterGuids;

		MaterialInstanceDynamic->GetAllScalarParameterInfo(MaterialsParameterInfos, ParameterGuids);
		for (int32 ParameterIndex = 0; ParameterIndex < MaterialsParameterInfos.Num(); ParameterIndex++)
		{
			float Value = 0;
			if (MaterialInstanceDynamic->GetScalarParameterValue(MaterialsParameterInfos[ParameterIndex].Name, Value, true))
			{
				UE_LOG(LogReadyPlayerMe, Warning, TEXT("Param: %s [%s] = %f"), *MaterialsParameterInfos[ParameterIndex].Name.ToString(), *ParameterGuids[ParameterIndex].ToString(), Value);
				MaterialInstance->SetScalarParameterValueEditorOnly(MaterialsParameterInfos[ParameterIndex], Value);
			}
		}

		MaterialInstanceDynamic->GetAllVectorParameterInfo(MaterialsParameterInfos, ParameterGuids);
		for (int32 ParameterIndex = 0; ParameterIndex < MaterialsParameterInfos.Num(); ParameterIndex++)
		{
			FLinearColor Value = FLinearColor::Black;
			if (MaterialInstanceDynamic->GetVectorParameterValue(MaterialsParameterInfos[ParameterIndex].Name, Value, true))
			{
				UE_LOG(LogReadyPlayerMe, Warning, TEXT("Param: %s [%s] = %s"), *MaterialsParameterInfos[ParameterIndex].Name.ToString(), *ParameterGuids[ParameterIndex].ToString(), *Value.ToString());
				MaterialInstance->SetVectorParameterValueEditorOnly(MaterialsParameterInfos[ParameterIndex], Value);
			}
		}

		MaterialInstanceDynamic->GetAllTextureParameterInfo(MaterialsParameterInfos, ParameterGuids);
		for (int32 ParameterIndex = 0; ParameterIndex < MaterialsParameterInfos.Num(); ParameterIndex++)
		{
			UTexture* TextureParam = nullptr;
			if (MaterialInstanceDynamic->GetTextureParameterValue(MaterialsParameterInfos[ParameterIndex].Name, TextureParam, true))
			{
			    if (IsTransient(TextureParam))
			    {
			        if (!TextureParam->Source.IsValid())
			        {
			            UTexture2D* Texture2D = Cast<UTexture2D>(TextureParam);
			            if (Texture2D)
			            {
			                FTexturePlatformData* PlatformData = Texture2D->GetPlatformData();
			                if (PlatformData && PlatformData->Mips.IsValidIndex(0))
			                {
			                    const void* Data = PlatformData->Mips[0].BulkData.LockReadOnly();
			                    const int32 Width = PlatformData->Mips[0].SizeX;
			                    const int32 Height = PlatformData->Mips[0].SizeY;
			                    int32 ImageBytes = Width * Height * 4; // Assuming 4 bytes per pixel for BGRA8
			                	
			                    TArray<uint8> ImageData;
			                    ImageData.AddUninitialized(ImageBytes);
			                	
			                    FMemory::Memcpy(ImageData.GetData(), Data, ImageBytes);
			                	
			                    PlatformData->Mips[0].BulkData.Unlock();

			                    TextureParam->Source.Init(Width, Height, 1, 1, TSF_BGRA8, ImageData.GetData());
			                }
			            }
			        }
			    	
			        const FString TextureName = AssetNameGenerator->GenerateTextureName(TextureParam, Material, MaterialPath, MaterialsParameterInfos[ParameterIndex].Name.ToString());
			        if (!TextureName.IsEmpty())
			        {
			            SaveUObject(TextureParam, TextureName);
			        }
			    }

			    UE_LOG(LogReadyPlayerMe, Warning, TEXT("Param: %s [%s] = %s"), *MaterialsParameterInfos[ParameterIndex].Name.ToString(), *ParameterGuids[ParameterIndex].ToString(), *TextureParam->GetFullName());
			    MaterialInstance->SetTextureParameterValueEditorOnly(MaterialsParameterInfos[ParameterIndex], TextureParam);
			}

		}
		OutMaterial = MaterialInstance;
		return SaveUObject(MaterialInstance, MaterialPath);
	}
	else if (IsTransient(Material))
	{
		OutMaterial = Material;
		return SaveUObject(Material, MaterialPath);
	}

	return false;
}

bool FRpmAssetSaver::SaveStaticMeshAsset(UStaticMesh* StaticMesh, const FString& StaticMeshPath, TSharedPtr<FAssetNameGenerator> AssetNameGenerator)
{
	// If no generator is provided, create a default instance
	if (!AssetNameGenerator.IsValid())
	{
		AssetNameGenerator = MakeShared<FAssetNameGenerator>();
	}
	if (!StaticMesh)
	{
		return false;
	}

	if (StaticMeshPath.IsEmpty())
	{
		return false;
	}

	if (!StaticMesh->GetMeshDescription(0))
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("The StaticMesh has no MeshDescription"));
		return false;
	}

	// now fix materials
	int32 MaterialIndex = 0;
	TArray<FStaticMaterial> NewMaterials = StaticMesh->GetStaticMaterials();
	for (const FStaticMaterial& StaticMaterial : StaticMesh->GetStaticMaterials())
	{
		UMaterialInterface* Material = StaticMaterial.MaterialInterface;
		UMaterialInterface* OutMaterial = nullptr;
		const FString NewMaterialName = AssetNameGenerator->GenerateMaterialName(Material, MaterialIndex, StaticMaterial.MaterialSlotName.ToString());
		if (!NewMaterialName.IsEmpty())
		{
			if (SaveMaterialAsset(Material, NewMaterialName, OutMaterial, AssetNameGenerator))
			{
				NewMaterials[MaterialIndex].MaterialInterface = OutMaterial;
			}
		}
		MaterialIndex++;
	}

	StaticMesh->SetStaticMaterials(NewMaterials);

	return SaveUObject(StaticMesh, StaticMeshPath);
}

bool FRpmAssetSaver::SaveSkeletalMeshAsset(USkeletalMesh* SkeletalMesh, const FString& SkeletalMeshPath, const FString& SkeletonPath, const FString& PhysicsAssetPath, TSharedPtr<FAssetNameGenerator> AssetNameGenerator)
{
	// If no generator is provided, create a default instance
	if (!AssetNameGenerator.IsValid())
	{
		AssetNameGenerator = MakeShared<FAssetNameGenerator>();
	}
	if (!SkeletalMesh)
	{
		return false;
	}

	if (!SkeletonPath.IsEmpty())
	{
		if (!SaveUObject(SkeletalMesh->GetSkeleton(), SkeletonPath))
		{
			return false;
		}
	}

	FSkeletalMeshModel* ImportedResource = SkeletalMesh->GetImportedModel();
	if (!ImportedResource)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Unable to GetImportedModel()"));
		return false;
	}

	UE_LOG(LogReadyPlayerMe, Log, TEXT("ImportedResource: %d"), ImportedResource->LODModels.Num());

	FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	if (!RenderData)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Unable to access RenderData"));
		return false;
	}

	//IMeshBuilderModule& MeshBuilderModule = IMeshBuilderModule::GetForRunningPlatform();

	const int32 NumLODs = RenderData->LODRenderData.Num();
	for (int32 LODIndex = 0; LODIndex < NumLODs; LODIndex++)
	{
		ImportedResource->LODModels[LODIndex].ActiveBoneIndices = RenderData->LODRenderData[LODIndex].ActiveBoneIndices;
		ImportedResource->LODModels[LODIndex].RequiredBones = RenderData->LODRenderData[LODIndex].RequiredBones;
		ImportedResource->LODModels[LODIndex].NumVertices = RenderData->LODRenderData[LODIndex].GetNumVertices();
		FMultiSizeIndexContainerData MultiSizeIndexContainer;
		RenderData->LODRenderData[LODIndex].MultiSizeIndexContainer.GetIndexBufferData(MultiSizeIndexContainer);
		ImportedResource->LODModels[LODIndex].IndexBuffer = MultiSizeIndexContainer.Indices;
		ImportedResource->LODModels[LODIndex].NumTexCoords = RenderData->LODRenderData[LODIndex].GetNumTexCoords();

		UE_LOG(LogReadyPlayerMe, Log, TEXT("LOD %d has %d vertices and %d sections"), LODIndex, ImportedResource->LODModels[LODIndex].NumVertices, ImportedResource->LODModels[LODIndex].Sections.Num());
		const int32 NumSections = RenderData->LODRenderData[LODIndex].RenderSections.Num();
		for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
		{
			const FSkelMeshRenderSection& RenderSection = RenderData->LODRenderData[LODIndex].RenderSections[SectionIndex];

			FSkelMeshSection& Section = ImportedResource->LODModels[LODIndex].Sections[SectionIndex];
			Section.BaseIndex = RenderSection.BaseIndex;
			Section.BaseVertexIndex = RenderSection.BaseVertexIndex;
			Section.bCastShadow = RenderSection.bCastShadow;
			Section.bDisabled = RenderSection.bDisabled;
			Section.BoneMap = RenderSection.BoneMap;
			Section.bRecomputeTangent = RenderSection.bRecomputeTangent;
			Section.MaterialIndex = RenderSection.MaterialIndex;
			Section.MaxBoneInfluences = RenderSection.MaxBoneInfluences;
			Section.NumTriangles = RenderSection.NumTriangles;
			Section.NumVertices = RenderSection.NumVertices;
			Section.bUse16BitBoneIndex = RenderData->LODRenderData[LODIndex].SkinWeightVertexBuffer.Use16BitBoneIndex();

			for (uint32 Index = RenderSection.BaseIndex; Index < RenderSection.BaseIndex + (RenderSection.NumTriangles * 3); Index++)
			{
				const uint32 VertexIndex = ImportedResource->LODModels[LODIndex].IndexBuffer[Index];
				FSoftSkinVertex SkinVertex;
				SkinVertex.Position = RenderData->LODRenderData[LODIndex].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(VertexIndex);
				SkinVertex.TangentX = RenderData->LODRenderData[LODIndex].StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentX(VertexIndex);
				SkinVertex.TangentY = RenderData->LODRenderData[LODIndex].StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentY(VertexIndex);
				SkinVertex.TangentZ = RenderData->LODRenderData[LODIndex].StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(VertexIndex);
				for (uint32 UVIndex = 0; UVIndex < ImportedResource->LODModels[LODIndex].NumTexCoords; UVIndex++)
				{
					SkinVertex.UVs[UVIndex] = RenderData->LODRenderData[LODIndex].StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIndex, UVIndex);
				}
				for (int32 InfluenceIndex = 0; InfluenceIndex < MAX_TOTAL_INFLUENCES; InfluenceIndex++)
				{
					SkinVertex.InfluenceBones[InfluenceIndex] = RenderData->LODRenderData[LODIndex].SkinWeightVertexBuffer.GetBoneIndex(VertexIndex, InfluenceIndex);
					SkinVertex.InfluenceWeights[InfluenceIndex] = RenderData->LODRenderData[LODIndex].SkinWeightVertexBuffer.GetBoneWeight(VertexIndex, InfluenceIndex);
				}
				Section.SoftVertices.Add(SkinVertex);
			}
		}
	}

	// now fix materials
	int32 MaterialIndex = 0;
	TArray<FSkeletalMaterial> NewMaterials = SkeletalMesh->GetMaterials();
	for (const FSkeletalMaterial& SkeletalMaterial : SkeletalMesh->GetMaterials())
	{
		UMaterialInterface* Material = SkeletalMaterial.MaterialInterface;
		UMaterialInterface* OutMaterial = nullptr;
		const FString NewMaterialName = AssetNameGenerator->GenerateMaterialName(Material, MaterialIndex, SkeletalMaterial.MaterialSlotName.ToString());
		if (!NewMaterialName.IsEmpty())
		{
			if (SaveMaterialAsset(Material, NewMaterialName, OutMaterial, AssetNameGenerator))
			{
				NewMaterials[MaterialIndex].MaterialInterface = OutMaterial;
			}
		}
		MaterialIndex++;
	}

	SkeletalMesh->SetMaterials(NewMaterials);

	SkeletalMesh->Build();

	return SaveUObject(SkeletalMesh, SkeletalMeshPath);
}