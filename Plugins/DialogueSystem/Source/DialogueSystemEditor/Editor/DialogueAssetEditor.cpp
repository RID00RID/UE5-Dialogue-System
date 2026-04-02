
#include "DialogueAssetEditor.h"

#include "GraphEditorActions.h"
#include "DialogueSystem/Core/DialogueAsset.h"
#include "DialogueSystem/Core/BaseDialogueNode.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/DialogueAssetGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Nodes/BaseDialogueGraphNode.h"


namespace DialogueAssetEditorTab
{
	const FName GraphTab = FName("GraphTab");
	const FName DetailsTab = FName("DetailsTab");
};

#pragma region ToolKit

FName FDialogueAssetEditor::GetToolkitFName() const
{
	return FName("Dialogue Asset Editor");
}

FText FDialogueAssetEditor::GetBaseToolkitName() const
{
	return GetToolkitName();
}

FString FDialogueAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Action Attribute Asset Editor");
}

FLinearColor FDialogueAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(FColor::Orange);
}



#pragma endregion

#pragma region Editor

void FDialogueAssetEditor::InitDialogueAssetEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UDialogueAsset* InDialogueAsset)
{
	DialogueAssetEd = InDialogueAsset;

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("DialogueAssetEditorLayout_v2")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
#if ENGINE_MAJOR_VERSION < 5
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
#endif // #if ENGINE_MAJOR_VERSION < 5
			->SetOrientation(Orient_Vertical)
			->Split(FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(1.f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.70f)
					->SetHideTabWell(true)
					->AddTab(DialogueAssetEditorTab::GraphTab, ETabState::OpenedTab)

				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.30f)
					->SetHideTabWell(true)
					->AddTab(DialogueAssetEditorTab::DetailsTab, ETabState::OpenedTab)

				)

			)
		);
	
	InitAssetEditor(Mode, InitToolkitHost, FName("ActionAttributeSetApp"), StandaloneDefaultLayout, true, true, DialogueAssetEd, false);
	RegenerateMenusAndToolbars();
}

void FDialogueAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(DialogueAssetEd);
}

FString FDialogueAssetEditor::GetReferencerName() const
{
	if (DialogueAssetEd)
	{
		return DialogueAssetEd->GetName();
		
	}
	return "Error Referencer Name";
}

void FDialogueAssetEditor::SaveAsset_Execute()
{
	if (IsValid(DialogueAssetEd->EdGraph))
	{

		UDialogueAssetGraph* EdGraph = Cast<UDialogueAssetGraph>(DialogueAssetEd->EdGraph);
		check(EdGraph != nullptr);

		EdGraph->RebuildGraph();
		EdGraph->NotifyGraphChanged();
	}
	FWorkflowCentricApplication::SaveAsset_Execute();
}

#pragma endregion

#pragma region TabSpawner

void FDialogueAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(FText::FromString(TEXT("Dialogue Asset Editor")));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
	
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(DialogueAssetEditorTab::GraphTab, FOnSpawnTab::CreateSP(this, &FDialogueAssetEditor::SpawnTab_Graph))
	.SetDisplayName(FText::FromString(TEXT("Dialogue Asset Graph")))
	.SetGroup(WorkspaceMenuCategoryRef)
	.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("GraphEditor.EventGraph_16x")));

	InTabManager->RegisterTabSpawner(DialogueAssetEditorTab::DetailsTab, FOnSpawnTab::CreateSP(this, &FDialogueAssetEditor::SpawnTab_Details))
	.SetDisplayName(FText::FromString(TEXT("Details")))
	.SetGroup(WorkspaceMenuCategoryRef)
	.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("GraphEditor.EventGraph_16x")));
}

void FDialogueAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(DialogueAssetEditorTab::GraphTab);
	InTabManager->UnregisterTabSpawner(DialogueAssetEditorTab::DetailsTab);
}

TSharedRef<SDockTab> FDialogueAssetEditor::SpawnTab_Graph(const FSpawnTabArgs& Args)
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString("Dialogue Asset Graph");

	CreateCommandList();


	SGraphEditor::FGraphEditorEvents InEvents = {};
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FDialogueAssetEditor::OnSelectedNodesChanged);
	InEvents.OnVerifyTextCommit = FOnNodeVerifyTextCommit::CreateSP(this, &FDialogueAssetEditor::OnVerifyNodeTextCommit);
	
	if (DialogueAssetEd->EdGraph == nullptr)
	{
		DialogueAssetEd->EdGraph = Cast<UDialogueAssetGraph>(FBlueprintEditorUtils::CreateNewGraph(DialogueAssetEd, NAME_None, UDialogueAssetGraph::StaticClass(), UDialogueAssetGraphSchema::StaticClass()));
		DialogueAssetEd->EdGraph->bAllowDeletion = false;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = DialogueAssetEd->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*DialogueAssetEd->EdGraph);
	}

	FocusedGraphEditor = SNew(SGraphEditor)
		.AdditionalCommands(ToolkitCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(DialogueAssetEd->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);

	
	return SNew(SDockTab).Label(FText::FromString("Graph"))[ FocusedGraphEditor.ToSharedRef() ];
}

TSharedRef<SDockTab> FDialogueAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	FDetailsViewArgs DetailsArgs;
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DialogueAssetProperties = PropertyModule.CreateDetailView(DetailsArgs);
	DialogueAssetProperties->SetObject(DialogueAssetEd);

	return SNew(SDockTab).Label(FText::FromString("Details"))[DialogueAssetProperties.ToSharedRef()];
}

#pragma endregion

#pragma region Command

void FDialogueAssetEditor::CreateCommandList()
{
	GraphEditorCommands = ToolkitCommands;
	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphCommands = FGraphEditorCommands::Get();

	GraphEditorCommands->MapAction(
		GenericCommands.Delete, FExecuteAction::CreateSP(this, &FDialogueAssetEditor::DeleteSelectedNodes), FCanExecuteAction::CreateSP(this, &FDialogueAssetEditor::CanDeleteNodes));
}

void FDialogueAssetEditor::DeleteSelectedNodes()
{
	if (!DialogueAssetEd) return;
	const FScopedTransaction Transaction(FText::FromString(TEXT("Delete Selected Node")));
	FocusedGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = FocusedGraphEditor->GetSelectedNodes();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);

		UBaseDialogueGraphNode* StateNode = Cast<UBaseDialogueGraphNode>(Node);
		/*if (StateNode && StateNode->IsSubNode())
		{
			UDialogueGraphNode* ParentNode = StateNode->GetParentNode();
			ParentNode->RemoveSubNode(StateNode);
			if (ParentNode)
			{
				ParentNode->ReconstructNode();
			}
		}*/
		
		if (Node && Node->CanUserDeleteNode())
		{
			FBlueprintEditorUtils::RemoveNode(nullptr, Node, true);
		}
	}

	OnSelectedNodesChanged({});
}

FGraphPanelSelectionSet FDialogueAssetEditor::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (FocusedGraphEditor)
	{
		CurrentSelection = FocusedGraphEditor->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FDialogueAssetEditor::OnSelectedNodesChanged(const TSet<UObject*>& Nodes)
{
	DialogueAssetProperties->SetObject(DialogueAssetEd);
	
	/*TArray<UObject*> Selection;

	if (Nodes.Num() != 0)
	{
		for (TSet<class UObject*>::TConstIterator SetIt(Nodes); SetIt; ++SetIt)
		{
			if (UBaseDialogueGraphNode* GraphTaskNode = Cast<UBaseDialogueGraphNode>(*SetIt))
			{
				UDialogueNode* UIState = GraphTaskNode->GetNodeData<UDialogueNode>();
				if (UIState)
				{
					Selection.Add(UIState);
				}
			}
		}

		DialogueAssetProperties->SetObjects(Selection);
	}
	else
	{
		DialogueAssetProperties->SetObject(DialogueAssetEd);
	}*/
}

bool FDialogueAssetEditor::OnVerifyNodeTextCommit(const FText& Text, UEdGraphNode* EdGraphNode, FText& OutErrorMessage)
{
	if (Text.IsEmpty())
	{
		OutErrorMessage = FText::FromString("Error: the state name cannot be empty");
		return false;
	}

	if (!DialogueAssetEd)
	{
		OutErrorMessage = FText::FromString("Error: No Asset");
		return false;
	}

	if (!DialogueAssetEd->EdGraph)
	{
		OutErrorMessage = FText::FromString("Error: No Asset Graph");
		return false;
	}

	for (auto Node : DialogueAssetEd->EdGraph->Nodes)
	{
		if (!Node) continue;
		if (Node->GetNodeTitle(ENodeTitleType::Type::FullTitle).ToString() == Text.ToString())
		{
			OutErrorMessage =  FText::FromString("Error: Such a state name already exists");
			return false;
		}
	}
	
	return true;
}

#pragma endregion
