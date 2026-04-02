// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueChoiceGraphNode.h"

#include "DialogueSystemEditor/Editor/FDialogueEditorStyle.h"

UDialogueChoiceGraphNode::UDialogueChoiceGraphNode()
{
	bCanRenameNode = false;
	bCanDeleteNode = true;
}

void UDialogueChoiceGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  "exec", FName(), TEXT(""));
}

#pragma region Style

FText UDialogueChoiceGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Dialogue Choice");
}

FText UDialogueChoiceGraphNode::GetTooltipText() const
{
	return FText::FromString("Dialogue with responses");
}

FLinearColor UDialogueChoiceGraphNode::GetNodeBodyTintColor() const
{
	return FLinearColor(FColor::FromHex("009ADD"));
}
	
#pragma endregion

#pragma region View Content

TSharedRef<SWidget> UDialogueChoiceGraphNode::TitleContent()
{
	return  SNew(SButton)
	.VAlign(VAlign_Center)
	.ButtonStyle(FDialogueEditorStyle::Get(), "DialogButton")
	.ButtonColorAndOpacity(FLinearColor(FColor::FromHex("007CB1")))
	.OnClicked_UObject(this, &UDialogueChoiceGraphNode::AddNewChoice)
	[
		SNew(STextBlock)
			.Text(FText::FromString("+ Add New Choice"))
			.Justification(ETextJustify::Center)
	];
}

UDialogChoiceResponse* UDialogueChoiceGraphNode::GetChoiceDataByPinId(FGuid Id)
{
	if (ChoiceData.Contains(Id))
	{
		return ChoiceData[Id];
	}
	return nullptr;
}

#pragma endregion

#pragma region Choices


FReply UDialogueChoiceGraphNode::AddNewChoice()
{
	UEdGraphPin* NewPin = CreatePin(EGPD_Output, FName("Choice"), FName(), TEXT(""));
	if (!NewPin)  FReply::Handled();

	if (ChoiceData.Contains(NewPin->PinId)) FReply::Handled();
	if (UDialogChoiceResponse* NewResponse = NewObject<UDialogChoiceResponse>(this, UDialogChoiceResponse::StaticClass()))
	{
		ChoiceData.Add(NewPin->PinId, NewResponse);
		NewPin->DefaultObject = NewResponse;	
	}
	ReconstructNode();
	if (GetGraph())
	{
		GetGraph()->Modify();
		GetGraph()->NotifyNodeChanged(this);
	}
	return FReply::Handled();
}


#pragma endregion 
