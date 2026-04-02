// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueSystemEditor.h"


#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"


#include "DialogueSystemEditor/Editor/FDialogueEditorStyle.h"
#include "DialogueSystemEditor/Editor/Factoryes/DialogueAssetFactory.h"
#include "DialogueSystemEditor/Editor/Factoryes/FDialogueGraphNodeFactory.h"
#include "DialogueSystemEditor/Editor/Factoryes/FDialoguePinFactory.h"


#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FDialogueSystemEditor"

EAssetTypeCategories::Type FDialogueSystemEditor::DialogueAssetCategory = static_cast<EAssetTypeCategories::Type>(0);

void FDialogueSystemEditor::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	DialogueAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Dialogue System")), FText::FromString(TEXT("Dialogue System")));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_DialogueAssetAction()));

	FDialogueEditorStyle::Initialize();

	GraphNodeFactory = MakeShareable(new FDialogueGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphNodeFactory);

	TSharedPtr<FDialoguePinFactory> Factory = MakeShareable(new FDialoguePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(Factory);
}

void FDialogueSystemEditor::ShutdownModule()
{
	FDialogueEditorStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogueSystemEditor, DialogueSystemEditor);