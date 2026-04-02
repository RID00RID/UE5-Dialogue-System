// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "BaseDialogueGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UBaseDialogueGraphNode : public UEdGraphNode
{
	GENERATED_BODY()


public:

	virtual bool CanUserDeleteNode() const override { return bCanDeleteNode; };
	
#pragma region Style

public:
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeBodyTintColor() const override;

	
#pragma endregion

#pragma region View Content

public:

	virtual TSharedRef<SWidget> TitleContent() { return SNew(SBox); };
	virtual TSharedRef<SWidget> CustomContent(TSharedPtr<IPropertyRowGenerator> InPropertyRow) { return SNew(SBox); };

	TSharedPtr<IPropertyHandle> FindPropertyByName(TSharedPtr<IPropertyRowGenerator> RowGenerator, const FName & InName);
	
#pragma endregion 

#pragma region Data

public:

	template<class T>
	T* GetNodeData() const {return Cast<T>(NodeData); }
	void SetNodeData(UObject* NewData) { NodeData = NewData; };
	
protected:

	UPROPERTY()
	UObject* NodeData;
	
#pragma endregion

#pragma region Properties

protected:
	
	bool bCanDeleteNode = false;
	
#pragma endregion 

	
};


USTRUCT()
struct DIALOGUESYSTEMEDITOR_API FAssetSchemaAction_DialogueNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public:
	FAssetSchemaAction_DialogueNode() : NodeTemplate(nullptr) {}

	FAssetSchemaAction_DialogueNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	virtual auto PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) -> UEdGraphNode* override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	TObjectPtr<UBaseDialogueGraphNode> NodeTemplate;
};