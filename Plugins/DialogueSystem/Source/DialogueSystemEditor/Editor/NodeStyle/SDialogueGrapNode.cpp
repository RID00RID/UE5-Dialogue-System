
#include "SDialogueGrapNode.h"

#include "IDetailPropertyRow.h"
#include "IDetailTreeNode.h"
#include "IPropertyRowGenerator.h"
#include "IPropertyTable.h"
#include "PropertyCustomizationHelpers.h"
#include "DialogueSystem/Core/BaseDialogueNode.h"
#include "DialogueSystemEditor/Editor/FDialogueEditorStyle.h"
#include "DialogueSystemEditor/Editor/Nodes/BaseDialogueGraphNode.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"


void SDialogueGrapNode::Construct(const FArguments& InArgs, UBaseDialogueGraphNode* InNode)
{
	this->GraphNode = InNode;
}

void SDialogueGrapNode::SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel)
{
	SGraphNode::SetOwner(OwnerPanel);
	UpdateGraphNode();
}

void SDialogueGrapNode::UpdateGraphNode()
{

	if (UBaseDialogueGraphNode* Node = CastChecked<UBaseDialogueGraphNode>(GraphNode))
	{
		if (Node->GetNodeData<UObject>())
		{
			FPropertyRowGeneratorArgs RowArgs;
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			RowGenerator = PropertyModule.CreatePropertyRowGenerator(RowArgs);
			RowGenerator->SetObjects({ Node->GetNodeData<UObject>() });
		}
	}

	
	SubNodeBox.Reset();
	LeftNodeBox.Reset();
	RightNodeBox.Reset();


	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SVerticalBox> NodeBody;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	
	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage(FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.DialogNodeBody"))
			.Padding(2)
			.BorderBackgroundColor(this, &SDialogueGrapNode::GetNodeBodyColor)
			[
				SNew(SBox)
				.MinDesiredWidth(150)
				.MinDesiredHeight(50)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.VAlign(VAlign_Center)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(SBox)
							.WidthOverride(28)
							.HeightOverride(28)
							[
								SNew(SBorder)
								.BorderImage(this, &SDialogueGrapNode::GetNodeIcon)
							]
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1)
						.Padding(5, 0)
						.VAlign(VAlign_Center)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(InlineEditableText, SInlineEditableTextBlock)
								.Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
								.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 9))
								.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
								.OnVerifyTextChanged(this, &SDialogueGrapNode::OnVerifyNodeNameChanged)
								.OnTextCommitted(this, &SDialogueGrapNode::OnChangeNodeName)
								.IsReadOnly(this, &SDialogueGrapNode::IsNameReadOnly)
								.IsSelected(this, &SDialogueGrapNode::IsSelectedExclusively)
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
							+ SVerticalBox::Slot()
							[
								SNew(STextBlock)
									.Text(this, &SDialogueGrapNode::GetNodeToolTip)
									.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 6))
							]
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(5, 0, 0, 2)
						[
							TitlePanel()
						]
					]
					+ SVerticalBox::Slot()
					.FillHeight(1)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SBorder)
						.Padding(0)
						.BorderBackgroundColor(FColor::FromHex("0A0A0BFF"))
						.BorderImage(FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.DialogNodeContent"))
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								CreateNodeContentArea()
						    ]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SBox)
								.Padding(2.f)
								[
									SAssignNew(PropertyContent, SVerticalBox)
								]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SBox)
								.Padding(2.f)
								[
									SAssignNew(SubNodeBox, SVerticalBox)
								]
							]
						]
					]
				]
			]
		];


	if (RowGenerator.IsValid())
	{
		for (TSharedRef<IDetailTreeNode> RootNode : RowGenerator->GetRootTreeNodes())
		{
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

				
				
				// фильтр по метадате
				if (PropHandle->HasMetaData("EditStateNode"))
				{
					TSharedPtr<SWidget> NameWidget  = PropHandle->CreatePropertyNameWidget();
					TSharedPtr<SWidget> ValueWidget = PropHandle->CreatePropertyValueWidget();

					PropertyContent->AddSlot()
					.Padding(2,0,7,0)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(SBox)
							.MinDesiredWidth(100.f)
							[
								NameWidget.ToSharedRef()
							]
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.WidthOverride(150.f)
							[
								ValueWidget.ToSharedRef()
							]
						]
					];
				}
			}
		}
	}

	CreatePinWidgets();
}

TSharedRef<SWidget> SDialogueGrapNode::CreateNodeContentArea()
{
	return  SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("NoBorder"))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					// LEFT
					SAssignNew(LeftNodeBox, SVerticalBox)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.HAlign(HAlign_Fill)
				[
					// RIGHT
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			Content()
		];
}

TSharedRef<SWidget> SDialogueGrapNode::TitlePanel()
{
	if (UBaseDialogueGraphNode* DialogNode = Cast<UBaseDialogueGraphNode>(GraphNode))
	{
		return DialogNode->TitleContent();
	}
	return SNew(SBox);
}

TSharedRef<SWidget> SDialogueGrapNode::Content()
{
	if (UBaseDialogueGraphNode* DialogNode = Cast<UBaseDialogueGraphNode>(GraphNode))
	{
		return DialogNode->CustomContent(RowGenerator);
	}
	return SNew(SBox);
}

#pragma region Node Title

bool SDialogueGrapNode::OnVerifyNodeNameChanged(const FText& InText, FText& OutErrorMessage) const
{
	bool bValid(true);
	
	if ((GetEditableNodeTitle() != InText.ToString()) && OnVerifyTextCommit.IsBound())
	{
		bValid = OnVerifyTextCommit.Execute(InText, GraphNode, OutErrorMessage);
	}

	if( OutErrorMessage.IsEmpty() )
	{
		OutErrorMessage = FText::FromString(TEXT("Error"));
	}
	
	return bValid;
}

void SDialogueGrapNode::OnChangeNodeName(const FText& InText, ETextCommit::Type CommitInfo)
{
	UBaseDialogueGraphNode* Node = Cast<UBaseDialogueGraphNode>(GraphNode);
	if (Node != nullptr && Node->GetNodeData<UDialogueNode>())
	{
		Node->GetNodeData<UDialogueNode>()->NodeName = FName(InText.ToString());
	}
}

FText SDialogueGrapNode::GetNodeToolTip() const
{
	UBaseDialogueGraphNode* Node = Cast<UBaseDialogueGraphNode>(GraphNode);
	if (Node != nullptr)
	{
		return Node->GetTooltipText();
	}

	return FText::FromString("");
}

const FSlateBrush* SDialogueGrapNode::GetNodeIcon() const
{
	return FDialogueEditorStyle::GetBrush("DialogueEditor.Icon.StartDialog");
}

#pragma endregion
