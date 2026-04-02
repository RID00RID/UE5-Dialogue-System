// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDialogueGraphNode.h"
#include "DialogueCloseGraphNode.generated.h"

/**
 * 
 */


UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueCloseGraphNode : public UBaseDialogueGraphNode
{
	GENERATED_BODY()


public:

	UDialogueCloseGraphNode();

	virtual void AllocateDefaultPins() override;

#pragma region Style

public:
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeBodyTintColor() const override;

#pragma endregion
};
