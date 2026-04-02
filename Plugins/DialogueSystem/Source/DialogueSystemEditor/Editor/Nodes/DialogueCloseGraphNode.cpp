// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueCloseGraphNode.h"

UDialogueCloseGraphNode::UDialogueCloseGraphNode()
{
	bCanRenameNode = false;
	bCanDeleteNode = true;
}

void UDialogueCloseGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  "exec", FName(), TEXT(""));
}

#pragma region Style

FText UDialogueCloseGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Dialogue End");
}

FText UDialogueCloseGraphNode::GetTooltipText() const
{
	return FText::FromString("End Dialogue Action");
}

FLinearColor UDialogueCloseGraphNode::GetNodeBodyTintColor() const
{
	return FLinearColor(FColor::FromHex("C52333"));
}

#pragma endregion
