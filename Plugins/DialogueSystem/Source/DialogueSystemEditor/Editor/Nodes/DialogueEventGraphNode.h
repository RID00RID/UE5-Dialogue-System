// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDialogueGraphNode.h"
#include "DialogueEventGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEventGraphNode : public UBaseDialogueGraphNode
{
	GENERATED_BODY()

public:

	UDialogueEventGraphNode();

	virtual void AllocateDefaultPins() override;

#pragma region Style

public:
	
	virtual FText GetTooltipText() const override;
	
#pragma endregion
};
