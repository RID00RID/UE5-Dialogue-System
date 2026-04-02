#pragma once

#include "EdGraphUtilities.h"

class DIALOGUESYSTEMEDITOR_API FDialogueGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
