// Fill out your copyright notice in the Description page of Project Settings.


#include "RootDialogueGraphNode.h"

URootDialogueGraphNode::URootDialogueGraphNode()
{
	bCanRenameNode = false;
	bCanDeleteNode = false;
}

#pragma region Style

FText URootDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Start Dialogue");
}

FText URootDialogueGraphNode::GetTooltipText() const
{
	return FText::FromString("Start Dialogue Node");
}

#pragma endregion
