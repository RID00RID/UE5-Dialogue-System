// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DialogueSystemCore.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueAsset.generated.h"

/**
 * 
 */


UCLASS()
class DIALOGUESYSTEM_API UDialogueAsset : public UObject
{
	GENERATED_BODY()

	friend class UDialogueNode;
	friend class UDialogueManager;
	
#pragma region Editor Graph

public:

	//UFUNCTION(BlueprintPure, Category = "UIState")
	//APlayerController* GetPlayerController() const;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	class UEdGraph* EdGraph;

#endif

public:
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DialogueAsset")
	class UDialogueNode* StartDialogueNode;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DialogueAsset")
	TMap<FGuid, class UDialogueNode*> AllNodes;
	
#pragma endregion


#pragma region Dialogue

public:

	void InitDialogueAsset(class UDialogueManager* NewDialogManager,  FName StartNodeName);

	class UDialogueManager* GetDialogueManager() const { return DialogueManager; };
	
	FDialogueNodeData GetCurrentDialogueNode();
	
	bool GetNextNode(FDialogueNodeData& NodeData);
	bool ChangeNextNode(FGuid NextNode,  FDialogueNodeData& NodeData);

	void CloseDialogue();

	
protected:

	UDialogueNode* FindNodeById(FGuid NodeID);

protected:
	
	UPROPERTY()
	UDialogueNode* CurrentDialogueNode;

	UPROPERTY()
	class UDialogueManager* DialogueManager;
	
	
#pragma endregion 
};
