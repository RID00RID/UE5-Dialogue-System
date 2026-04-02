// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "DialogueCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class DIALOGUESYSTEM_API UDialogueCondition : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DialogueCondition)
	bool HasCondition(class UDialogueAsset* Dialogue);
	virtual bool HasCondition_Implementation(class UDialogueAsset* Dialogue) { return true; };
};


UCLASS()
class DIALOGUESYSTEM_API UHasTag : public UDialogueCondition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DialogueCondition)
	FGameplayTag ConditionTag;

	virtual bool HasCondition_Implementation(class UDialogueAsset* Dialogue) override
	{
		return false;
	};
};
