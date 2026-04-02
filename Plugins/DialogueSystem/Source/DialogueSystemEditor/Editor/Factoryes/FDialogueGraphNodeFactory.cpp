#include "FDialogueGraphNodeFactory.h"

#include "DialogueSystemEditor/Editor/Nodes/BaseDialogueGraphNode.h"
#include "DialogueSystemEditor/Editor/NodeStyle/SDialogueGrapNode.h"

TSharedPtr<class SGraphNode> FDialogueGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UBaseDialogueGraphNode* EdGrapNode = Cast<UBaseDialogueGraphNode>(Node))
	{
		return SNew(SDialogueGrapNode, EdGrapNode);
	}
	
	return FGraphPanelNodeFactory::CreateNode(Node);
}
