// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueAssetGraph.h"

#include "DialogueSystem/Core/BaseDialogueNode.h"

// Dialogue Node
#include "CookOnTheFly.h"
#include "DialogueSystem/Core/DialogueAsset.h"
#include "DialogueSystem/Core/DialogueCondition.h"
#include "DialogueSystemEditor/Editor/Nodes/BaseDialogueGraphNode.h"
#include "DialogueSystemEditor/Editor/Nodes/DialogueChoiceGraphNode.h"
#include "DialogueSystemEditor/Editor/Nodes/DialogueCloseGraphNode.h"
#include "DialogueSystemEditor/Editor/Nodes/RootDialogueGraphNode.h"
#include "DialogueSystemEditor/Editor/Nodes/DialogueEventGraphNode.h"
#include "DialogueSystemEditor/Editor/Nodes/ExecuteDialogueGraphNode.h"
#include "DialogueSystemEditor/Editor/Nodes/MainDialogueGraphNode.h"


#define DISALLOW_CONNECT FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Not implemented by this schema"))
#define ALLOW_CONNECT FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Allow connect pins"));

class UDialogueAsset* UDialogueAssetGraph::GetDialogueAsset() const
{
	return CastChecked<UDialogueAsset>(GetOuter());
}

void UDialogueAssetGraph::Clear()
{
	UDialogueAsset* DialogueAsset = GetDialogueAsset();
	if (DialogueAsset)
	{
		if (DialogueAsset->StartDialogueNode)
		{
			DialogueAsset->StartDialogueNode->Clear();
		}

		for (auto Node : DialogueAsset->AllNodes)
		{
			if (!Node.Value) continue;
			Node.Value->Clear();
		}
		
		DialogueAsset->StartDialogueNode = nullptr;
		DialogueAsset->AllNodes.Reset();


	}
}

void UDialogueAssetGraph::RebuildGraph()
{
	Clear();
	UDialogueAsset* DialogueAsset = GetDialogueAsset();
	if (!DialogueAsset) return;

	for (int i = 0; i < Nodes.Num(); ++i)
	{

		if (URootDialogueGraphNode* RootEdNode = Cast<URootDialogueGraphNode>(Nodes[i]))
		{
			if (RootEdNode->GetNodeData<UDialogueNode>() == nullptr)
				continue;

			DialogueAsset->StartDialogueNode = RootEdNode->GetNodeData<UDialogueNode>();
		}
		
		if (UBaseDialogueGraphNode* EdNode = Cast<UBaseDialogueGraphNode>(Nodes[i]))
		{
			if (EdNode->GetNodeData<UDialogueNode>() == nullptr)
				continue;

			UDialogueNode* Entity = EdNode->GetNodeData<UDialogueNode>();

			DialogueNodeMap.Add(Entity, EdNode);
			DialogueAsset->AllNodes.Add(Entity->NodeID, Entity);

			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];

				if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
					continue;

				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					if (UBaseDialogueGraphNode* EdNode_Child = Cast<UBaseDialogueGraphNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						if (UDialogueNode* ChildDialogueNodeData = EdNode_Child->GetNodeData<UDialogueNode>())
						{
							ChildDialogueNodeData->SetParentNode(Entity->NodeID);
							Entity->AddNextNode(ChildDialogueNodeData->NodeID);
							
							/*if (EdNode->IsA(URootDialogueGraphNode::StaticClass()))
							{
								DialogueAsset->StartDialogueNode = ChildDialogueNodeData;
							}*/
						}

					
					}
				}
			}
		}

		if (UDialogueChoiceGraphNode* Choice_EdNode = Cast<UDialogueChoiceGraphNode>(Nodes[i]))
		{
			if (Choice_EdNode->GetNodeData<UDialogueChoiceNode>() == nullptr)
				continue;

			UDialogueChoiceNode* ChoiceEntity = Choice_EdNode->GetNodeData<UDialogueChoiceNode>();
			
			for (int PinIdx = 0; PinIdx < Choice_EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = Choice_EdNode->Pins[PinIdx];

				if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
					continue;

				if (UDialogChoiceResponse* ChoiceResponse = Choice_EdNode->GetChoiceDataByPinId(Pin->PinId))
				{
					FDialogueResponse NewResponse = FDialogueResponse();
					NewResponse.Response = ChoiceResponse->ResponseText;
					NewResponse.Condition = ChoiceResponse->Condition;
					NewResponse.Events = ChoiceResponse->Events;
				
					for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
					{
						if (UBaseDialogueGraphNode* EdNode = Cast<UBaseDialogueGraphNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
						{
							if (EdNode->GetNodeData<UDialogueNode>() == nullptr)
								continue;

							NewResponse.NextNode = EdNode->GetNodeData<UDialogueNode>()->NodeID;
							if (NewResponse.Condition)
							{
								NewResponse.Condition->Rename(nullptr, EdNode->GetNodeData<UDialogueNode>(), REN_DontCreateRedirectors | REN_DoNotDirty);
							}
						}
					}

								
					ChoiceEntity->AddNewResponse(NewResponse);
				}

			}
		}
	}

	for (auto Node : DialogueAsset->AllNodes)
	{
		if (!Node.Value) continue;
		Node.Value->Rename(nullptr, DialogueAsset, REN_DontCreateRedirectors | REN_DoNotDirty);
	}
	
}

void UDialogueAssetGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<URootDialogueGraphNode> ResultNodeCreator(Graph);
	URootDialogueGraphNode* RootNode = ResultNodeCreator.CreateNode();
	RootNode->NodePosX = 0;
	RootNode->NodePosY = 0;
	UDialogueNode* NodeObject = NewObject<UDialogueNode>(RootNode, UDialogueNode::StaticClass());
	RootNode->SetNodeData(NodeObject);
	ResultNodeCreator.Finalize();
	SetNodeMetaData(RootNode, FNodeMetadata::DefaultGraphNode);
}

void UDialogueAssetGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{

	TSharedPtr<FAssetSchemaAction_DialogueNode> NewNodeAction(new FAssetSchemaAction_DialogueNode(FText::FromString("Dialogue"), FText::FromString("Custom Event"), FText::FromString("State Node Tooltip"), 0));
	NewNodeAction->NodeTemplate = NewObject<UDialogueEventGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	NewNodeAction->NodeTemplate->SetNodeData(NewObject<UDialogueNode>(NewNodeAction->NodeTemplate, UDialogueNode::StaticClass()));
	ContextMenuBuilder.AddAction(NewNodeAction);


	TSharedPtr<FAssetSchemaAction_DialogueNode> NewDialogueAction(new FAssetSchemaAction_DialogueNode(FText::FromString("Dialogue"), FText::FromString("Dialogue"), FText::FromString("State Node Tooltip"), 0));
	NewDialogueAction->NodeTemplate = NewObject<UMainDialogueGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	NewDialogueAction->NodeTemplate->SetNodeData(NewObject<UMainDialogueNode>(NewNodeAction->NodeTemplate, UMainDialogueNode::StaticClass()));
	ContextMenuBuilder.AddAction(NewDialogueAction);

	TSharedPtr<FAssetSchemaAction_DialogueNode> NewDialogueChoiceNodeAction(new FAssetSchemaAction_DialogueNode(FText::FromString("Dialogue"), FText::FromString("Dialogue Choice"), FText::FromString("State Node Tooltip"), 0));
	NewDialogueChoiceNodeAction->NodeTemplate = NewObject<UDialogueChoiceGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	NewDialogueChoiceNodeAction->NodeTemplate->SetNodeData(NewObject<UDialogueChoiceNode>(NewNodeAction->NodeTemplate, UDialogueChoiceNode::StaticClass()));
	ContextMenuBuilder.AddAction(NewDialogueChoiceNodeAction);

	
	TSharedPtr<FAssetSchemaAction_DialogueNode> NewReturnDialogueNodeAction(new FAssetSchemaAction_DialogueNode(FText::FromString("Dialogue"), FText::FromString("Return Dialogue"), FText::FromString("State Node Tooltip"), 0));
	NewReturnDialogueNodeAction->NodeTemplate = NewObject<UExecuteDialogueGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	ContextMenuBuilder.AddAction(NewReturnDialogueNodeAction);

	TSharedPtr<FAssetSchemaAction_DialogueNode> NewDialogueEndNodeAction(new FAssetSchemaAction_DialogueNode(FText::FromString("Dialogue"), FText::FromString("Dialogue End"), FText::FromString("State Node Tooltip"), 0));
	NewDialogueEndNodeAction->NodeTemplate = NewObject<UDialogueCloseGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
	NewDialogueEndNodeAction->NodeTemplate->SetNodeData(NewObject<UCloseDialogEvent>(NewNodeAction->NodeTemplate, UCloseDialogEvent::StaticClass()));
	ContextMenuBuilder.AddAction(NewDialogueEndNodeAction);
}

void UDialogueAssetGraphSchema::GetContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);

	if (Context->Pin != nullptr && Context->Pin->GetOwningNode() != nullptr && Context->Pin->PinType.PinCategory == "Choice")
	{
		FToolMenuSection& Section = Menu->AddSection("DialogueGraphPinActions", FText::FromString("Choice Pin Actions"));

		Section.AddMenuEntry(
			"RemovePin",
			FText::FromString("Remove pin"),
			FText::FromString("Remove Selected pin"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateLambda([Pin = Context->Pin]()
				{
					if (Pin && Pin->GetOwningNode())
					{
						UEdGraphNode* Node = Pin->GetOwningNode();
						Node->Modify();

						Node->RemovePin(const_cast<UEdGraphPin*>(Pin));
						Node->GetGraph()->NotifyGraphChanged();
					}
				})
			)
		);
	}
}

const FPinConnectionResponse UDialogueAssetGraphSchema::CanCreateConnection(const UEdGraphPin* A,
                                                                            const UEdGraphPin* B) const
{
	if (!A || !B) return DISALLOW_CONNECT;
	if (A == B) return DISALLOW_CONNECT;

	//if (A->PinType.PinCategory != B->PinType.PinCategory) return DISALLOW_CONNECT;

	if (A->Direction == EGPD_Input && B->Direction == EGPD_Input) return DISALLOW_CONNECT;
	if (A->Direction == EGPD_Output && B->Direction == EGPD_Output) return DISALLOW_CONNECT;
	if (A->GetOwningNode() == B->GetOwningNode()) return DISALLOW_CONNECT;
	
	//if (B->Direction == EGPD_Input && B->LinkedTo.Num() > 0) return DISALLOW_CONNECT;
	
	//if (A->LinkedTo.Num() > 0 || B->LinkedTo.Num() > 0) return DISALLOW_CONNECT;

	const UEdGraphNode* A_GraphNode = Cast<UEdGraphNode>(A->GetOwningNode());
	const UEdGraphNode* B_GraphNode = Cast<UEdGraphNode>(B->GetOwningNode());
	if (!A_GraphNode || !B_GraphNode) return DISALLOW_CONNECT;

	

	return ALLOW_CONNECT;
}

bool UDialogueAssetGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	if (Super::TryCreateConnection(A, B))
	{
		UEdGraphNode* NodeA = Cast<UEdGraphNode>(A->GetOwningNode());
		UEdGraphNode* NodeB = Cast<UEdGraphNode>(B->GetOwningNode());
		
		if (NodeA && NodeB)
		{
			Super::TryCreateConnection(A, B);
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool UDialogueAssetGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	return true;
}

FLinearColor UDialogueAssetGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FColor::White;
}
