#include "EditorAssetLoader.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "RpmActor.h"
#include "RpmNextGen.h"

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
			LoadAssetToWorldAsURpmActor(GltfAsset, Asset.Id);
			GltfAsset->RemoveFromRoot();
		}
	}
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
				// TODO cleanup or remove
				//NewActor->LoadAsset(FRpmAsset(), GltfAsset);
			}
			UE_LOG(LogReadyPlayerMe, Log, TEXT("Successfully loaded GLB asset into the editor world"));
			return;
		}
		
		UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to spawn ARpmActor in the editor world"));
	}
	UE_LOG(LogReadyPlayerMe, Error, TEXT("Failed to load GLB asset from file"));
}
