// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainDialogueGraphNode.h"
#include "DialogueChoiceGraphNode.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, DefaultToInstanced)
class UDialogChoiceResponse : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ChoiceResponse")
	FText ResponseText = FText::FromString("Response");

	//ditStateNode
	
	UPROPERTY(EditAnywhere, Instanced, Category = "ChoiceResponse", meta=(ResponseEvent))
	class UDialogueCondition* Condition;

	UPROPERTY(EditAnywhere, Instanced, Category = "ChoiceResponse", meta=(ResponseEvent))
	TArray<class UDialogueChoiceEvent*> Events; 
};


UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueChoiceGraphNode : public UMainDialogueGraphNode
{
	GENERATED_BODY()


public:

	UDialogueChoiceGraphNode();

	virtual void AllocateDefaultPins() override;

#pragma region Style

public:
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeBodyTintColor() const override;

	
#pragma endregion

#pragma region View Content

public:

	virtual TSharedRef<SWidget> TitleContent() override;

#pragma endregion

#pragma region Choices

public:

	UDialogChoiceResponse* GetChoiceDataByPinId(FGuid Id);

protected:
	
	FReply AddNewChoice();

protected:

	UPROPERTY()
	TMap<FGuid, UDialogChoiceResponse*> ChoiceData;
	
#pragma endregion 
};
