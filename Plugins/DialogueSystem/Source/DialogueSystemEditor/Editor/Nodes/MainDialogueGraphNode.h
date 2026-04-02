// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDialogueGraphNode.h"
#include "MainDialogueGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UMainDialogueGraphNode : public UBaseDialogueGraphNode
{
	GENERATED_BODY()

public:

	UMainDialogueGraphNode();

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

	FText GetSpeakerName() const;
	FSlateColor SpeakerNameTextColor() const;
	void SpeakerNameCommitted(const FText& Text, ETextCommit::Type Arg) const;
	bool IsSpeakerNameReadOnly() const;

	
	FText GetDialogueText() const;
	FSlateColor DialogueTextTextColor() const;
	void DialogueTextCommitted(const FText& Text, ETextCommit::Type Arg) const;
	bool IsDialogueTextReadOnly() const;
	
protected:
	
	TSharedPtr<IPropertyHandle> SpeakerHandler;
	TSharedPtr<IPropertyHandle> DialogueTextHandler;
	
#pragma endregion 

};
