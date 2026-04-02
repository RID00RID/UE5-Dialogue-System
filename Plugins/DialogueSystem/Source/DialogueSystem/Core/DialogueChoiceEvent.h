// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueChoiceEvent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class DIALOGUESYSTEM_API UDialogueChoiceEvent : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Event)
	void ExecuteEvent(class UDialogueAsset* Dialogue);
	virtual void ExecuteEvent_Implementation(class UDialogueAsset* Dialogue) {  };
};


UCLASS()
class DIALOGUESYSTEM_API UBroadcastQuestEvent : public UDialogueChoiceEvent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Event)
	FName EventName;

	virtual void ExecuteEvent_Implementation(class UDialogueAsset* Dialogue) override;
};
