// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuteDialogueGraphNode.h"

#include "DialogueEventGraphNode.h"
#include "DialogueSystem/Core/BaseDialogueNode.h"

UExecuteDialogueGraphNode::UExecuteDialogueGraphNode()
{
	bCanRenameNode = false;
	bCanDeleteNode = true;
}

void UExecuteDialogueGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  "exec", FName(), TEXT(""));
}

FText UExecuteDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (UDialogueNode* Data =GetNodeData<UDialogueNode>())
	{
		return FText::FromString("Return To " + Data->NodeName.ToString());
	}
	return FText::FromString("Return To");
}

FText UExecuteDialogueGraphNode::GetTooltipText() const
{
	return FText::FromString("Call Selected Dialogue Node");
}

FLinearColor UExecuteDialogueGraphNode::GetNodeBodyTintColor() const
{
	return FLinearColor(FColor::FromHex("1C644C"));
}

TSharedRef<SWidget> UExecuteDialogueGraphNode::CustomContent(TSharedPtr<IPropertyRowGenerator> InPropertyRow)
{
	Options.Reset();
	if (GetGraph())
	{
		Options.Add(MakeShareable(new FString("None")));
		for (auto Node : GetGraph()->Nodes)
		{
			if (!Node) continue;

			if (UDialogueEventGraphNode* EdNode = Cast<UDialogueEventGraphNode>(Node))
			{
				if (EdNode->GetClass() != UDialogueEventGraphNode::StaticClass())
					continue;
				
				UDialogueNode* EventNodeData = EdNode->GetNodeData<UDialogueNode>();
				if (EdNode->GetNodeData<UDialogueNode>() == nullptr)
					continue;
				
				Options.Add(MakeShareable(new FString(EventNodeData->NodeName.ToString())));
			}
		}
	}


	if (UDialogueNode* Data =GetNodeData<UDialogueNode>())
	{
		CurrentItem = MakeShareable(new FString(Data->NodeName.ToString()));
	}
	else
	{
		CurrentItem = MakeShareable(new FString("None"));
	}
	
	return SNew(SBox)
		.Padding(5,0)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.HeightOverride(25.F)
		[
			SNew(SComboBox<FComboItemType>)
			.OptionsSource(&Options)
			.OnSelectionChanged_UObject(this, &UExecuteDialogueGraphNode::OnSelectionChanged)
			.OnGenerateWidget_UObject(this, &UExecuteDialogueGraphNode::MakeWidgetForOption)
			.InitiallySelectedItem(CurrentItem)
			[
				SNew(STextBlock)
					.Text(FText::FromString(*CurrentItem))
			]
		];
}

TSharedRef<SWidget> UExecuteDialogueGraphNode::MakeWidgetForOption(FComboItemType InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

void UExecuteDialogueGraphNode::OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type)
{
	if (!NewValue) return;
	CurrentItem = NewValue;
	
	if (*NewValue == "None")
	{
		NodeData = nullptr;
		Modify();
		GetGraph()->NotifyNodeChanged(this);
		return;
	}

	if (GetGraph())
	{
		for (auto Node : GetGraph()->Nodes)
		{
			if (!Node) continue;

			if (UDialogueEventGraphNode* EdNode = Cast<UDialogueEventGraphNode>(Node))
			{
				if (EdNode->GetNodeData<UDialogueNode>() == nullptr)
					continue;

				if (EdNode->GetNodeData<UDialogueNode>()->NodeName.ToString() == *CurrentItem)
				{
					NodeData = EdNode->GetNodeData<UDialogueNode>();
					Modify();
					GetGraph()->NotifyNodeChanged(this);
					return;
				}
			}
		}
	}
}
