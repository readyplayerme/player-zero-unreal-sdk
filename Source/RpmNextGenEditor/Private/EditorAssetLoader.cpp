#include "EditorAssetLoader.h"
#include "AssetSaver/AssetNameGenerator.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "RpmActor.h"
#include "RpmNextGen.h"
#include "AssetSaver/RpmAssetSaver.h"

FEditorAssetLoader::FEditorAssetLoader()
{
	SkeletonToCopy = nullptr;
	OnGlbLoaded.BindRaw(this, &FEditorAssetLoader::HandleGlbLoaded);
	GltfConfig = new FglTFRuntimeConfig();
	GltfConfig->TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
}

FEditorAssetLoader::~FEditorAssetLoader()
{
}

void FEditorAssetLoader::HandleGlbLoaded(const FRpmAsset& Asset, const TArray<unsigned char>& Data)
{
	UglTFRuntimeAsset* GltfAsset = nullptr;
	if (!Data.IsEmpty())
	{
		GltfAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, *GltfConfig);
		if (GltfAsset)
		{
			GltfAsset->AddToRoot();
			SaveAsUAsset(GltfAsset, Asset.Id);
			LoadAssetToWorldAsURpmActor(GltfAsset, Asset.Id);
			GltfAsset->RemoveFromRoot();
		}
	}
	
}

USkeletalMesh* FEditorAssetLoader::SaveAsUAsset(UglTFRuntimeAsset* GltfAsset, const FString& LoadedAssetId) const
{
	const FglTFRuntimeSkeletonConfig SkeletonConfig = FglTFRuntimeSkeletonConfig();
	USkeleton* Skeleton = GltfAsset->LoadSkeleton(0, SkeletonConfig);
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load skeleton for %s"), *LoadedAssetId);
		return nullptr;
	}
	FglTFRuntimeSkeletalMeshConfig MeshConfig = FglTFRuntimeSkeletalMeshConfig();
	MeshConfig.Skeleton = Skeleton;

	USkeletalMesh* SkeletalMesh = GltfAsset->LoadSkeletalMeshRecursive(TEXT(""), {}, MeshConfig);

	if (!IsValid(SkeletalMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load skeletal mesh for %s. 1"), *LoadedAssetId);
		return nullptr;
	}

	// Ensure proper UObject flags to avoid garbage collection
	SkeletalMesh->SetFlags(RF_Public | RF_Standalone);
	SkeletalMesh->SetSkeleton(Skeleton);
	Skeleton->SetPreviewMesh(SkeletalMesh);

	const FString CoreAssetPath = FString::Printf(TEXT("/Game/ReadyPlayerMe/CharacterModels/%s/"), *LoadedAssetId);
	const FString SkeletonAssetPath = FString::Printf(TEXT("%s%s_Skeleton"), *CoreAssetPath, *LoadedAssetId);
	const FString SkeletalMeshAssetPath = FString::Printf(TEXT("%s%s_SkeletalMesh"), *CoreAssetPath, *LoadedAssetId);
	UE_LOG(LogTemp, Log, TEXT("Saving SkeletalMesh to path: %s"), *SkeletalMeshAssetPath);
	UE_LOG(LogTemp, Log, TEXT("Saving Skeleton to path: %s"), *SkeletonAssetPath);
	FRpmAssetSaver::SaveSkeletalMeshAsset(SkeletalMesh, SkeletalMeshAssetPath, SkeletonAssetPath, TEXT(""));

	UE_LOG(LogReadyPlayerMe, Log, TEXT("Character model saved: %s"), *LoadedAssetId);
	return SkeletalMesh;
}

void FEditorAssetLoader::LoadBCharacterStyleAsset(const FRpmAsset& Asset)
{
	LoadGlb(Asset, Asset.Id);
}

void FEditorAssetLoader::LoadAssetToWorldAsURpmActor(UglTFRuntimeAsset* GltfAsset, FString AssetId)
{
	this->LoadAssetToWorld(AssetId, GltfAsset);
}


void FEditorAssetLoader::LoadAssetToWorld(const FString& AssetId, UglTFRuntimeAsset* GltfAsset)
{
	if (!GEditor)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("GEditor is not available."));
		return;
	}
	
	UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
	if (!EditorWorld)
	{
		UE_LOG(LogReadyPlayerMe, Error, TEXT("No valid editor world found."));
		return;
	}

	if (GltfAsset)
	{
		const FTransform Transform = FTransform::Identity;


		ARpmActor* NewActor = EditorWorld->SpawnActorDeferred<ARpmActor>(ARpmActor::StaticClass(), Transform);

		if (NewActor)
		{
			NewActor->SetFlags(RF_Transient);
			NewActor->Rename(*AssetId);
			if (SkeletonToCopy)
			{
				NewActor->SkeletalMeshConfig.SkeletonConfig.CopyRotationsFrom = SkeletonToCopy;
			}
			NewActor->FinishSpawning(Transform);
			NewActor->DispatchBeginPlay();
			GEditor->SelectNone(true, true, true);
			GEditor->SelectActor(NewActor, true, true, false, true);

			// Register the actor in the editor world and update the editor
			GEditor->SelectActor(NewActor, true, true);
			GEditor->EditorUpdateComponents();
			if (GltfAsset)
			{
				NewActor->LoadAsset(FRpmAsset(), GltfAsset);
			}
			UE_LOG(LogReadyPlayerMe, Log, TEXT("Successfully loaded GLB asset into the editor world"));
			return;
		}
		
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to spawn ARpmActor in the editor world"));
	}
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load GLB asset from file"));
}
