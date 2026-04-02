// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class FDialogueSystemEditor : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
	
	//Asset Category
	static EAssetTypeCategories::Type DialogueAssetCategory;

private:

	TSharedPtr<class FDialogueGraphNodeFactory> GraphNodeFactory;
};
