// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueAsset.h"

#include "BaseDialogueNode.h"
#include "DialogueCondition.h"
#include "DialogueManager.h"


void UDialogueAsset::InitDialogueAsset(class UDialogueManager* NewDialogManager, FName StartNodeName)
{
	if (!NewDialogManager) return;
	DialogueManager = NewDialogManager;
	if (StartNodeName == NAME_None)
	{
		CurrentDialogueNode = StartDialogueNode;
		FDialogueNodeData Data = FDialogueNodeData();
		GetNextNode(Data);
		NewDialogManager->OnChangeDialogue.Broadcast(Data);
	}
	else
	{
		for (auto Node : AllNodes)
		{
			if (!IsValid(Node.Value)) continue;

			if (Node.Value->NodeName == StartNodeName)
			{
				CurrentDialogueNode = Node.Value;
				break;
			}
		}

		if (!IsValid(CurrentDialogueNode))
		{
			CurrentDialogueNode = StartDialogueNode;

		}
		
		FDialogueNodeData Data = FDialogueNodeData();
		GetNextNode(Data);
		NewDialogManager->OnChangeDialogue.Broadcast(Data);
		
	}

}

FDialogueNodeData UDialogueAsset::GetCurrentDialogueNode()
{
	
	if (IsValid(CurrentDialogueNode) && CurrentDialogueNode->GetNodeData().CurrentNode)
	{
		TArray<FDialogueResponse> NewResponses = {};
		FDialogueNodeData NodeData = CurrentDialogueNode->GetNodeData();
		for (auto ResponsesData : NodeData.Responses)
		{
			if (!IsValid(ResponsesData.Condition))
			{
				NewResponses.Add(ResponsesData);
				continue;
			}
				
			if (ResponsesData.Condition->HasCondition(this))
			{
				NewResponses.Add(ResponsesData);
			}
		}
		NodeData.Responses = NewResponses;
		return NodeData;
	}

	return FDialogueNodeData();
}

bool UDialogueAsset::GetNextNode(FDialogueNodeData& NodeData)
{
	if (!CurrentDialogueNode) return false;
	
	CurrentDialogueNode->ExecuteNode(this);
	UDialogueNode* NextNode = FindNodeById(CurrentDialogueNode->GetFirstNextNode());
	if (NextNode)
	{
		CurrentDialogueNode = NextNode;
		if (!CurrentDialogueNode->GetNodeData().CurrentNode)
		{
			return GetNextNode(NodeData);
		}
		else
		{
			TArray<FDialogueResponse> NewResponses = {};
			NodeData = CurrentDialogueNode->GetNodeData();
			for (auto ResponsesData : NodeData.Responses)
			{
				if (!IsValid(ResponsesData.Condition))
				{
					NewResponses.Add(ResponsesData);
					continue;
				}
				
				if (ResponsesData.Condition->HasCondition(this))
				{
					NewResponses.Add(ResponsesData);
				}
			}
			NodeData.Responses = NewResponses;
			return true;
		}
	}
	return false;
}


bool UDialogueAsset::ChangeNextNode(FGuid NextNode, FDialogueNodeData& NodeData)
{
	if (!FindNodeById(NextNode)) return false;
	CurrentDialogueNode = FindNodeById(NextNode);

	if (!CurrentDialogueNode->GetNodeData().CurrentNode)
	{
		return GetNextNode(NodeData);
	}
	else
	{
		TArray<FDialogueResponse> NewResponses = {};
		NodeData = CurrentDialogueNode->GetNodeData();
		for (auto ResponsesData : NodeData.Responses)
		{
			if (!IsValid(ResponsesData.Condition))
			{
				NewResponses.Add(ResponsesData);
				continue;
			}
				
			if (ResponsesData.Condition->HasCondition(this))
			{
				NewResponses.Add(ResponsesData);
			}
		}
		NodeData.Responses = NewResponses;
		return true;
	}
}

void UDialogueAsset::CloseDialogue()
{
	if (DialogueManager)
	{
		DialogueManager->CloseDialogue();
	}
	DialogueManager = nullptr;
}


UDialogueNode* UDialogueAsset::FindNodeById(FGuid NodeID)
{
	if (AllNodes.Contains(NodeID))
	{
		return AllNodes[NodeID];
	}
	
	return nullptr;
}


