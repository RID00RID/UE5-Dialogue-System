// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueSystemCore.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable)
struct FDialogueResponse
{
	GENERATED_BODY()

public:

	FDialogueResponse() : Response(FText()), NextNode(FGuid()), Condition(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Response")
	FText Response = FText::FromString("Response");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Response")
	FGuid NextNode;

	UPROPERTY(Instanced)
	class UDialogueCondition* Condition;

	UPROPERTY(Instanced)
	TArray<class UDialogueChoiceEvent*> Events;
	
};


USTRUCT(Blueprintable)
struct FDialogueNodeData
{
	GENERATED_BODY()

public:

	FDialogueNodeData() : CurrentNode(nullptr), SpeakerName(FText()), DialogueText(FText()), Responses({})
	{
		
	}

	UPROPERTY()
	class UDialogueNode* CurrentNode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Data")
	FText SpeakerName = FText::FromString("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Data")
	FText DialogueText = FText::FromString("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Data")
	TArray<FDialogueResponse> Responses;

};

UCLASS()
class DIALOGUESYSTEM_API UDialogueSystemCore : public UObject
{
	GENERATED_BODY()
};
