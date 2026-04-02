// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DialogueSystemEditor : ModuleRules
{
	public DialogueSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine"
		});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"DialogueSystem",
				
				
				"Kismet",
				"UnrealEd",
				"EditorFramework",
				"GraphEditor",
				"ApplicationCore",
				"EditorStyle",
				"Projects",
				"ToolMenus",
				"PropertyEditor",

				"UMG",
				"Slate",
				"SlateCore",
				"ToolWidgets",
				"InputCore",
			});
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
			});
	}
}
