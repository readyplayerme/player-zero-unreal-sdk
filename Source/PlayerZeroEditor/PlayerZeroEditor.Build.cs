// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlayerZeroEditor : ModuleRules
{
	public PlayerZeroEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"HTTP",
				"JsonUtilities",
				"PlayerZero",
				"EditorStyle",
				"glTFRuntime",
				"UnrealEd",
				"PropertyEditor",
				"Slate",
				"SlateCore",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Json", 
				"UMG",
				"ImageWrapper",
				"MeshBuilder",
				"SkeletalMeshUtilitiesCommon"
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
