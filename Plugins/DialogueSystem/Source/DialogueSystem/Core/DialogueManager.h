// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueAsset.h"
#include "Components/ActorComponent.h"
#include "DialogueManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseDialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenDialogue, UDialogueAsset*, NewDialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDialogue, const FDialogueNodeData&, DialogueNode);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGUESYSTEM_API UDialogueManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDialogueManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	static UDialogueManager* GetDialogueManager(AActor* FromActor);

#pragma region Dialogue

public:

	UFUNCTION(BlueprintCallable, Category = Dialogue)
	bool OpenDialogueAsset(UDialogueAsset* NewDialogue, FName StartDialogueNode = NAME_None);

	UFUNCTION(BlueprintPure, Category = Dialogue)
	FDialogueNodeData GetCurrentDialogueNode() const;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool NextDialogNode();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool ChangeNextDialogNode(FDialogueResponse Response);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void CloseDialogue();

public:

	UPROPERTY(BlueprintAssignable)
	FOnCloseDialogue OnCloseDialogue;
	
	UPROPERTY(BlueprintAssignable)
	FOnOpenDialogue OnOpenDialogue;

	UPROPERTY(BlueprintAssignable)
	FOnChangeDialogue OnChangeDialogue;
	
protected:
	
	UPROPERTY()
	UDialogueAsset* CurrentDialogue;
	
	
#pragma endregion 
};
