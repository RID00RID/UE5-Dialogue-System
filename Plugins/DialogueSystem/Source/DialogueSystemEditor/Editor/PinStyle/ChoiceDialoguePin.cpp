#include "ChoiceDialoguePin.h"

#include "BlueprintActionDatabase.h"
#include "IDetailPropertyRow.h"
#include "IDetailTreeNode.h"
#include "IPropertyRowGenerator.h"
#include "ISinglePropertyView.h"
#include "SSimpleComboButton.h"
#include "DialogueSystemEditor/Editor/FDialogueEditorStyle.h"
#include "DialogueSystemEditor/Editor/Nodes/DialogueChoiceGraphNode.h"
#include "DialogueSystemEditor/Editor/Slate/SLocalizationSelected.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

void SChoiceDialoguePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	if (InPin->DefaultObject)
	{
		FPropertyRowGeneratorArgs RowArgs;
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		RowGenerator = PropertyModule.CreatePropertyRowGenerator(RowArgs);
		RowGenerator->SetObjects({ InPin->DefaultObject });

		FSinglePropertyParams PropertyParams;
		PropertyView = PropertyModule.CreateSingleProperty(InPin->DefaultObject, FName("Condition"), PropertyParams);

		
		if (PropertyView.IsValid())
		{
			FSimpleDelegate Delegate = FSimpleDelegate::CreateRaw(this, &SChoiceDialoguePin::OnConditionSelected);
			PropertyView->SetOnPropertyValueChanged(Delegate);
		}

		FDetailsViewArgs DetailsViewArgs;
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = false;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.bShowOptions = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowPropertyMatrixButton = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);
		DetailsView->SetObject( InPin->DefaultObject );

		DetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda(
		[](const FPropertyAndParent& PropertyAndParent)
		{
			return PropertyAndParent.Property.HasMetaData(TEXT("ResponseEvent"));
		}));
		
		/*if (PropertyView.IsValid())
		{
			if (PropertyView->GetPropertyHandle().IsValid())
			{
				UObject* Value = nullptr;
				PropertyView->GetPropertyHandle()->GetValue(Value);
				if (Value)
				{
					DetailsView->SetObject( Value);
				}
			}
		}*/


		
		

	}
	
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}



TSharedRef<SWidget> SChoiceDialoguePin::GetLabelWidget(const FName& InPinLabelStyle)
{
	ResponseTextHandle = FindPropertyByName(RowGenerator, FName("ResponseText"));
	ConditionHandle = FindPropertyByName(RowGenerator, FName("Test"));
	
	ConditionContent.Reset();
	TSharedPtr<SBox> Content = SNew(SBox);
	
	if (PropertyView.IsValid())
	{
		Content = SNew(SBox)
			.MinDesiredWidth(250)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				DetailsView.ToSharedRef()
			]
		];
	}


	TSharedRef<SWidget>  LWidget = SNew(SBox)
		.HAlign(HAlign_Right)
		[
	
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			.VAlign(VAlign_Center)
			.Padding(2,0)
			[
				SNew(SInlineEditableTextBlock)
				.Text(this, &SChoiceDialoguePin::GetResponseText)
				.ColorAndOpacity(this, &SChoiceDialoguePin::ResponseTextColor)
				.OnTextCommitted(this, &SChoiceDialoguePin::OnChangeResponseTextCommited)
				.IsReadOnly(this, &SChoiceDialoguePin::IsResponseTextReadOnly)
				
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SSimpleComboButton)
				.Icon(this, &SChoiceDialoguePin::GetConnditionIcon)
				.MenuContent()
				[
					SNew(SBox)
					.Padding(5)
					.WidthOverride(250)
					.VAlign(VAlign_Center)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0,2)
						[
							SNew(STextBlock).Text(FText::FromString("Condition"))
						]
						+SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBorder)
							.Padding(5,2)
							.BorderImage(FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody"))
							.BorderBackgroundColor(FLinearColor(FColor::FromHex("1A1A1A")))
							[
								Content.ToSharedRef()
							]
						]
					]
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SLocalizationSelected).Property(ResponseTextHandle)
			]
		];
	
	return LWidget;
}

FReply SChoiceDialoguePin::RemovePin() const
{
	UEdGraphPin* GraphPin = GetPinObj();
	if (GraphPin && !GraphPin->IsPendingKill())
	{
		if (UEdGraphNode* GraphNode = GraphPin->GetOwningNodeUnchecked())
		{
			GraphNode->RemovePin(GraphPin);
			GraphNode->ReconstructNode();
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}

const FSlateBrush* SChoiceDialoguePin::GetConnditionIcon() const
{
	if (PropertyView.IsValid())
	{
		UObject* Value = nullptr;
		PropertyView->GetPropertyHandle()->GetValue(Value);
		if (Value)
		{
			return FDialogueEditorStyle::GetBrush("DialogueEditor.Icon.ConditionActive");
		}
	}
	return FDialogueEditorStyle::GetBrush("DialogueEditor.Icon.Condition");
}

void SChoiceDialoguePin::OnConditionSelected() const
{
	if (!DetailsView.IsValid()) return;
	if (!PropertyView.IsValid()) return;
	
	if (PropertyView->GetPropertyHandle().IsValid())
	{
		UObject* Value = nullptr;
		PropertyView->GetPropertyHandle()->GetValue(Value);
		if (Value)
		{
			DetailsView->SetObject( Value);
		}
		else
		{
			DetailsView->SetObject( nullptr );
		}
	}
	
}

FText SChoiceDialoguePin::GetResponseText() const
{
	FText DialogueText = FText();
	if (ResponseTextHandle.IsValid())
	{
		ResponseTextHandle->GetValue(DialogueText);
	}
	if (DialogueText.IsEmpty())
	{
		return  FText::FromString("None");
	}
	return DialogueText;
}

FSlateColor SChoiceDialoguePin::ResponseTextColor() const
{
	if (ResponseTextHandle.IsValid())
	{
		FText TextValue =FText();
		ResponseTextHandle->GetValue(TextValue);
		if (TextValue.IsFromStringTable())
		{
			return FLinearColor(FColor::FromHex("404040"));
		}
	}
	return FLinearColor::White;
}

void SChoiceDialoguePin::OnChangeResponseTextCommited(const FText& InText, ETextCommit::Type CommitInfo) const
{
	if (ResponseTextHandle.IsValid())
	{
		ResponseTextHandle->SetValue(InText);
	}
}

bool SChoiceDialoguePin::IsResponseTextReadOnly() const
{
	if (ResponseTextHandle.IsValid())
	{
		FText TextValue =FText();
		ResponseTextHandle->GetValue(TextValue);
		if (TextValue.IsFromStringTable())
		{
			return true;
		}
	}
	return false;
}

TSharedPtr<IPropertyHandle> SChoiceDialoguePin::FindPropertyByName(const TSharedPtr<IPropertyRowGenerator>& InRowGenerator,const FName& InName)
{
	if (!InRowGenerator.IsValid())
	{
		return nullptr;
	}

	if (!InName.IsValid())
	{
		return nullptr;
	}

	if (InRowGenerator->GetRootTreeNodes().IsValidIndex(0))
	{
		TSharedRef<IDetailTreeNode> RootNode = InRowGenerator->GetRootTreeNodes()[0];
		TArray<TSharedRef<IDetailTreeNode>> ChildrenProperty;
		RootNode->GetChildren(ChildrenProperty);

		for (TSharedRef<IDetailTreeNode> Child : ChildrenProperty)
		{
			IDetailPropertyRow* PropertyRow = Child->GetRow().Get();
			if (!PropertyRow)
				continue;

			TSharedPtr<IPropertyHandle> PropHandle = PropertyRow->GetPropertyHandle();
			if (!PropHandle.IsValid())
				continue;

			if (PropHandle->GetProperty()->GetFName() == InName)
			{
				return PropHandle;
			}
		}
	}

	return nullptr;
}

