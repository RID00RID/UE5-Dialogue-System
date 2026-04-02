// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDialogueGraphNode.h"

#include "IDetailPropertyRow.h"
#include "IDetailTreeNode.h"
#include "IPropertyRowGenerator.h"
#include "DialogueSystem/Core/BaseDialogueNode.h"

#pragma region Style

FText UBaseDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (UDialogueNode* Node = GetNodeData<UDialogueNode>())
	{
		return FText::FromName(Node->NodeName);
	}
	return FText::FromString("DialogueGraphNode");
}

FText UBaseDialogueGraphNode::GetTooltipText() const
{
	return FText::FromString("DialogueGraphNode");
}

FLinearColor UBaseDialogueGraphNode::GetNodeBodyTintColor() const
{
	return FLinearColor(FColor::FromHex("309A4C"));
}

TSharedPtr<IPropertyHandle> UBaseDialogueGraphNode::FindPropertyByName(TSharedPtr<IPropertyRowGenerator> RowGenerator, const FName & InName)
{
	if (!RowGenerator.IsValid())
	{
		return nullptr;
	}

	if (!InName.IsValid())
	{
		return nullptr;
	}

	if (RowGenerator->GetRootTreeNodes().IsValidIndex(0))
	{
		TSharedRef<IDetailTreeNode> RootNode = RowGenerator->GetRootTreeNodes()[0];
		TArray<TSharedRef<IDetailTreeNode>> ChildrenProperty;
		RootNode->GetChildren(ChildrenProperty);

		for (TSharedRef<IDetailTreeNode> Child : ChildrenProperty)
		{
			IDetailPropertyRow* PropertyRow = Child->GetRow().Get();
			if (!PropertyRow)
				continue;

			TSharedPtr<IPropertyHandle> PropHandle = PropertyRow->GetPropertyHandle();
			if (!PropHandle.IsValid())
				continue;

			if (PropHandle->GetProperty()->GetFName() == InName)
			{
				return PropHandle;
			}
		}
	}

	return nullptr;
}

#pragma endregion



UEdGraphNode* FAssetSchemaAction_DialogueNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(FText::FromString("UITree Graph Editor: New Node"));
		ParentGraph->Modify();
		if (FromPin != nullptr)
			FromPin->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		if (NodeTemplate->GetNodeData<UObject>())
		{
			NodeTemplate->GetNodeData<UObject>()->SetFlags(RF_Transactional);
		}
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FAssetSchemaAction_DialogueNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}
