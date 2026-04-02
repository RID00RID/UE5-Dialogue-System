#pragma once
#include "FDialogueGraphNodeFactory.h"
#include "DialogueSystemEditor/Editor/PinStyle/ChoiceDialoguePin.h"


class DIALOGUESYSTEMEDITOR_API FDialoguePinFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override
	{
		if (InPin->PinType.PinCategory == "Choice")
		{
			return SNew(SChoiceDialoguePin, InPin);
		}
		return nullptr;
	}
};
