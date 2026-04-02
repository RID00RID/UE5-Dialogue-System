// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueEventGraphNode.h"
#include "RootDialogueGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API URootDialogueGraphNode : public UDialogueEventGraphNode
{
	GENERATED_BODY()

public:

	URootDialogueGraphNode();


#pragma region Style

public:
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	
#pragma endregion 
};
