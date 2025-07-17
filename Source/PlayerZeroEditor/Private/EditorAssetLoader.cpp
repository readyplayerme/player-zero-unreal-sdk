#include "EditorAssetLoader.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "PlayerZeroActor.h"
#include "PlayerZero.h"


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

void FEditorAssetLoader::HandleGlbLoaded(const FAsset& Asset, const TArray<unsigned char>& Data)
{
	UglTFRuntimeAsset* GltfAsset = nullptr;
	if (!Data.IsEmpty())
	{
		GltfAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, *GltfConfig);
		if (GltfAsset)
		{
			GltfAsset->AddToRoot();
			LoadAssetToWorldAsUPlayerZeroActor(GltfAsset, Asset.Id);
			GltfAsset->RemoveFromRoot();
		}
	}
	
}

void FEditorAssetLoader::LoadBaseModelAsset(const FAsset& Asset)
{
	LoadGlb(Asset, Asset.Id, false);
}

void FEditorAssetLoader::LoadAssetToWorldAsUPlayerZeroActor(UglTFRuntimeAsset* GltfAsset, FString AssetId)
{
	this->LoadAssetToWorld(AssetId, GltfAsset);
}


void FEditorAssetLoader::LoadAssetToWorld(const FString& AssetId, UglTFRuntimeAsset* GltfAsset)
{
	if (!GEditor)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("GEditor is not available."));
		return;
	}
	
	UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
	if (!EditorWorld)
	{
		UE_LOG(LogPlayerZero, Error, TEXT("No valid editor world found."));
		return;
	}

	if (GltfAsset)
	{
		const FTransform Transform = FTransform::Identity;


		APlayerZeroActor* NewActor = EditorWorld->SpawnActorDeferred<APlayerZeroActor>(APlayerZeroActor::StaticClass(), Transform);

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
				NewActor->LoadAsset(FAsset(), GltfAsset);
			}
			UE_LOG(LogPlayerZero, Log, TEXT("Successfully loaded GLB asset into the editor world"));
			return;
		}
		
		UE_LOG(LogPlayerZero, Error, TEXT("Failed to spawn APlayerZeroActor in the editor world"));
	}
	UE_LOG(LogPlayerZero, Error, TEXT("Failed to load GLB asset from file"));
}
