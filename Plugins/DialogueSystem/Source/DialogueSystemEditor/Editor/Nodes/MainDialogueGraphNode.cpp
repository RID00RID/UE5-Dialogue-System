// Fill out your copyright notice in the Description page of Project Settings.


#include "MainDialogueGraphNode.h"

#include "BlueprintActionDatabase.h"
#include "DialogueSystemEditor/Editor/FDialogueEditorStyle.h"
#include "DialogueSystemEditor/Editor/Slate/SLocalizationSelected.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"


UMainDialogueGraphNode::UMainDialogueGraphNode()
{
	bCanRenameNode = false;
	bCanDeleteNode = true;
}

void UMainDialogueGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  "exec", FName(), TEXT(""));
	CreatePin(EGPD_Output, "exec", FName(), TEXT(""));
}

#pragma region Style

FText UMainDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Dialogue");
}

FText UMainDialogueGraphNode::GetTooltipText() const
{
	return FText::FromString("State Node to display Dialogue");
}

FLinearColor UMainDialogueGraphNode::GetNodeBodyTintColor() const
{
	return FLinearColor(FColor::FromHex("E56F15"));
}
#pragma endregion

#pragma region View Content

TSharedRef<SWidget> UMainDialogueGraphNode::CustomContent(TSharedPtr<IPropertyRowGenerator> InPropertyRow)
{
	SpeakerHandler = FindPropertyByName(InPropertyRow, FName("SpeakerName"));
	DialogueTextHandler = FindPropertyByName(InPropertyRow, FName("DialogueText"));
	
	return SNew(SBox)
		.Padding(7, 2)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SVerticalBox)
		   +SVerticalBox::Slot()
			.Padding(0,2)
			.AutoHeight()
		   [
			   SNew(SHorizontalBox)
			   +SHorizontalBox::Slot()
			   .FillWidth(1.f)
			   [
				   SNew(SBorder)
				   .Padding(10,10)
				   .BorderImage(FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody"))
				   .BorderBackgroundColor(FLinearColor(FColor::FromHex("1A1A1A")))
				   [
					   SNew(SInlineEditableTextBlock)
					   .Text_UObject(this, &UMainDialogueGraphNode::GetSpeakerName)
					   .ColorAndOpacity_UObject(this, &UMainDialogueGraphNode::SpeakerNameTextColor)
					   .OnTextCommitted_UObject(this, &UMainDialogueGraphNode::SpeakerNameCommitted)
					   .IsReadOnly_UObject(this, &UMainDialogueGraphNode::IsSpeakerNameReadOnly)
				   ]
			   ]
			   +SHorizontalBox::Slot()
			   .AutoWidth()
			   [
				   SNew(SLocalizationSelected).Property(SpeakerHandler)
			   ]
		   ]
			+SVerticalBox::Slot()
			.Padding(0,2)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1.f)
				[
					SNew(SBorder)
					.Padding(10,10)
					.BorderImage(FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody"))
					.BorderBackgroundColor(FLinearColor(FColor::FromHex("1A1A1A")))
					[
						SNew(SInlineEditableTextBlock)
						.MultiLine(true)
						.Text_UObject(this, &UMainDialogueGraphNode::GetDialogueText)
						.ColorAndOpacity_UObject(this, &UMainDialogueGraphNode::DialogueTextTextColor)
						.OnTextCommitted_UObject(this, &UMainDialogueGraphNode::DialogueTextCommitted)
						.IsReadOnly_UObject(this, &UMainDialogueGraphNode::IsDialogueTextReadOnly)
					]
				]
				+SHorizontalBox::Slot()
			   .AutoWidth()
			   [
				   SNew(SLocalizationSelected).Property(DialogueTextHandler)
			   ]
			]
		];
}

FText UMainDialogueGraphNode::GetSpeakerName() const
{
	FText SpeakerName = FText();
	if (SpeakerHandler.IsValid())
	{
		SpeakerHandler->GetValue(SpeakerName);
	}
	if (SpeakerName.IsEmpty())
	{
		return FText::FromString("None");
	}
	return SpeakerName;
}

FSlateColor UMainDialogueGraphNode::SpeakerNameTextColor() const
{
	if (SpeakerHandler.IsValid())
	{
		FText TextValue =FText();
		SpeakerHandler->GetValue(TextValue);
		if (TextValue.IsFromStringTable())
		{
			return FLinearColor(FColor::FromHex("404040"));
		}
	}
	return FLinearColor::White;
}

void UMainDialogueGraphNode::SpeakerNameCommitted(const FText& Text, ETextCommit::Type Arg) const
{
	if (SpeakerHandler.IsValid())
	{
		SpeakerHandler->SetValue(Text);
	}
}

bool UMainDialogueGraphNode::IsSpeakerNameReadOnly() const
{
	if (SpeakerHandler.IsValid())
	{
		FText TextValue =FText();
		SpeakerHandler->GetValue(TextValue);
		if (TextValue.IsFromStringTable())
		{
			return true;
		}
	}
	return false;
}

FText UMainDialogueGraphNode::GetDialogueText() const
{
	FText DialogueText = FText();
	if (DialogueTextHandler.IsValid())
	{
		DialogueTextHandler->GetValue(DialogueText);
	}
	if (DialogueText.IsEmpty())
	{
		return  FText::FromString("None");
	}
	return DialogueText;
}

FSlateColor UMainDialogueGraphNode::DialogueTextTextColor() const
{
	if (DialogueTextHandler.IsValid())
	{
		FText TextValue =FText();
		DialogueTextHandler->GetValue(TextValue);
		if (TextValue.IsFromStringTable())
		{
			return FLinearColor(FColor::FromHex("404040"));
		}
	}
	return FLinearColor::White;
}

void UMainDialogueGraphNode::DialogueTextCommitted(const FText& Text, ETextCommit::Type Arg) const
{
	if (DialogueTextHandler.IsValid())
	{
		DialogueTextHandler->SetValue(Text);
	}
}

bool UMainDialogueGraphNode::IsDialogueTextReadOnly() const
{
	if (DialogueTextHandler.IsValid())
	{
		FText TextValue =FText();
		DialogueTextHandler->GetValue(TextValue);
		if (TextValue.IsFromStringTable())
		{
			return true;
		}
	}
	return false;
}

#pragma endregion 
