#pragma once


#include "SGraphNode.h"

class UBaseDialogueGraphNode;
class DIALOGUESYSTEMEDITOR_API SDialogueGrapNode : public SGraphNode
{

public:
	
	SLATE_BEGIN_ARGS(SDialogueGrapNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UBaseDialogueGraphNode* InNode);
	virtual void SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel) override;

	virtual void UpdateGraphNode() override;

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual TSharedRef<SWidget> TitlePanel();
	virtual TSharedRef<SWidget> Content();

#pragma region Node Title

private:
	
	virtual bool OnVerifyNodeNameChanged ( const FText& InText, FText& OutErrorMessage ) const;
	virtual void OnChangeNodeName(const FText& InText, ETextCommit::Type CommitInfo);

	FText GetNodeToolTip() const;
	const FSlateBrush* GetNodeIcon() const;
	
#pragma endregion

private:
	
	TSharedPtr<SVerticalBox> SubNodeBox;
	TSharedPtr<SVerticalBox> PropertyContent;
	TSharedPtr<IPropertyRowGenerator> RowGenerator;
};
