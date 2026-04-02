// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueAsset.h"
#include "DialogueManager.h"
#include "DialogueSystemCore.h"
#include "UObject/Object.h"
#include "BaseDialogueNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced)
class DIALOGUESYSTEM_API UDialogueNode : public UObject
{
	GENERATED_BODY()


#pragma region State

public:

	virtual void Clear() { ParentNode = FGuid(); NextNode.Reset(); };
	void SetParentNode(FGuid InParentNode) { ParentNode = InParentNode; };
	void AddNextNode(FGuid InNextNode) { NextNode.Add(InNextNode); };

	virtual FGuid GetFirstNextNode()
	{
		if (NextNode.IsValidIndex(0))
		{
			return NextNode[0];
		}
		return FGuid();
	};
	
	virtual FDialogueNodeData GetNodeData()
	{
		return FDialogueNodeData();
	}
	
	virtual void ExecuteNode(UDialogueAsset* Asset)
	{
		
	};
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIState")
	FGuid NodeID = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIState")
	FName NodeName;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIState")
	FGuid ParentNode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIState")
	TArray<FGuid> NextNode;
	
#pragma endregion
	
};



UCLASS()
class DIALOGUESYSTEM_API UCloseDialogEvent : public UDialogueNode
{
	GENERATED_BODY()

public:
	
	virtual void ExecuteNode(UDialogueAsset* Asset) override
	{
		if (!Asset) return;


		if (NextDialogue)
		{
			if (UDialogueManager* Manager = Asset->GetDialogueManager())
			{
				Manager->OpenDialogueAsset(NextDialogue, ExecNode);
			}
		}
		else
		{
			Asset->CloseDialogue();
		}
	};

#pragma region Dialogue

protected:

	UFUNCTION()
	TArray<FName> GetNodeNameOptions() const
	{
		TArray<FName> Names = { NAME_None };
		if (IsValid(NextDialogue))
		{
			for (auto Node : NextDialogue->AllNodes)
			{
				if (!Node.Value) continue;
				if (Node.Value->NodeName != NAME_None)
				{
					Names.Add(Node.Value->NodeName);
				}
			}
		}

		return Names;
	}
	
private:

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (EditStateNode, DisplayThumbnail=false))
	class UDialogueAsset* NextDialogue;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (EditStateNode, GetOptions="GetNodeNameOptions"))
	FName ExecNode;

	
#pragma endregion
	
};

UCLASS()
class DIALOGUESYSTEM_API UDialogEvents : public UDialogueNode
{
	GENERATED_BODY()

	
};


UCLASS()
class DIALOGUESYSTEM_API UMainDialogueNode : public UDialogueNode
{
	GENERATED_BODY()


public:

	virtual FDialogueNodeData GetNodeData() override
	{
		FDialogueNodeData Data = FDialogueNodeData();
		Data.CurrentNode = this;
		Data.SpeakerName = SpeakerName;
		Data.DialogueText = DialogueText;
		return Data;
	}

#pragma region Property

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DialogueNode")
	FText SpeakerName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DialogueNode", meta = (MultiLine=true))
	FText DialogueText;
	
#pragma endregion 
	
};


UCLASS()
class DIALOGUESYSTEM_API UDialogueChoiceNode : public UMainDialogueNode
{
	GENERATED_BODY()


public:

	virtual FGuid GetFirstNextNode() override { return FGuid(); };
	virtual FDialogueNodeData GetNodeData() override
	{
		FDialogueNodeData Data = FDialogueNodeData();
		Data.CurrentNode =this;
		Data.SpeakerName = SpeakerName;
		Data.DialogueText = DialogueText;
		Data.Responses = DialogueResponse;
		return Data;
	}
	
#pragma region Property

public:

	virtual void Clear() { Super::Clear(); DialogueResponse.Reset(); };
	void AddNewResponse(FDialogueResponse InResponse) { DialogueResponse.Add(InResponse); };
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DialogueNode")
	TArray<FDialogueResponse> DialogueResponse;
	
#pragma endregion 
};



