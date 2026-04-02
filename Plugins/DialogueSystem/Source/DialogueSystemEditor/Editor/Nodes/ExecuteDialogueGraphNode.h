// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDialogueGraphNode.h"
#include "ExecuteDialogueGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UExecuteDialogueGraphNode : public UBaseDialogueGraphNode
{
	GENERATED_BODY()

	typedef TSharedPtr<FString> FComboItemType;
	
public:

	UExecuteDialogueGraphNode();

	virtual void AllocateDefaultPins() override;

#pragma region Style

public:
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeBodyTintColor() const override;

#pragma endregion

#pragma region View Content

public:

	virtual TSharedRef<SWidget> CustomContent(TSharedPtr<IPropertyRowGenerator> InPropertyRow) override;

protected:

	TSharedRef<SWidget> MakeWidgetForOption(FComboItemType InOption);
	void OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type);
	
protected:

	FComboItemType CurrentItem;
	TArray<FComboItemType> Options;
	
#pragma endregion 
};
