#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"

class DIALOGUESYSTEMEDITOR_API SChoiceDialoguePin : public SGraphPin
{
	
public:
	SLATE_BEGIN_ARGS(SChoiceDialoguePin) {}
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:

	//~ Begin SGraphPin Interface
	virtual TSharedRef<SWidget> GetLabelWidget(const FName& InPinLabelStyle) override;
	//~ End SGraphPin Interface

	FReply RemovePin() const;
	
	const FSlateBrush* GetConnditionIcon() const;
	void OnConditionSelected() const;

	FText GetResponseText() const;
	FSlateColor ResponseTextColor() const;
	void OnChangeResponseTextCommited(const FText& InText, ETextCommit::Type CommitInfo) const;
	bool IsResponseTextReadOnly() const;


	static TSharedPtr<IPropertyHandle> FindPropertyByName(const TSharedPtr<IPropertyRowGenerator>& InRowGenerator, const FName & InName);
protected:

	TSharedPtr<IPropertyHandle> ResponseTextHandle;
	TSharedPtr<IPropertyHandle> ConditionHandle;

	TSharedPtr<SVerticalBox> ConditionContent;
	TSharedPtr<IPropertyRowGenerator> RowGenerator;
	TSharedPtr<IDetailsView> DetailsView;

	TSharedPtr<ISinglePropertyView>  PropertyView;
	TSharedPtr<ISinglePropertyView>  ChoiceEventView;
};
