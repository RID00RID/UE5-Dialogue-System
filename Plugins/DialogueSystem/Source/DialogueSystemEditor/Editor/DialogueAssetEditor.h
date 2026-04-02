#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FDialogueAssetEditor : public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient, public FGCObject
{
	
public:


#pragma region ToolKit

public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

#pragma endregion


#pragma region Editor

public:
	
	void InitDialogueAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UDialogueAsset* InDialogueAsset);
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

	virtual void SaveAsset_Execute() override;
	virtual bool CanSaveAssetAs() const override { return true; };

#pragma endregion

#pragma region TabSpawner

public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	
	TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

	
#pragma endregion

#pragma region Command

protected:

	virtual void CreateCommandList();

	//Delet node
	virtual bool CanDeleteNodes() const { return true; };
	virtual void DeleteSelectedNodes();

	//selected Nodes
	FGraphPanelSelectionSet GetSelectedNodes() const;
	void OnSelectedNodesChanged(const TSet<UObject*>& Nodes);

	bool OnVerifyNodeTextCommit(const FText& Text, UEdGraphNode* EdGraphNode, FText& OutErrorMessage);

#pragma endregion
	
#pragma region Property

private:

	TObjectPtr<class UDialogueAsset> DialogueAssetEd{ nullptr };

	TSharedPtr<SGraphEditor> FocusedGraphEditor;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TSharedPtr<class IDetailsView> DialogueAssetProperties;
	
#pragma endregion
};
