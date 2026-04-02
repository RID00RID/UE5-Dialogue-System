// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueEventGraphNode.h"

UDialogueEventGraphNode::UDialogueEventGraphNode()
{
	bCanRenameNode = true;
	bCanDeleteNode = true;
}

void UDialogueEventGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, "exec", FName(), TEXT("Start"));
}

FText UDialogueEventGraphNode::GetTooltipText() const
{
	return FText::FromString("Dialogue Event");
}
