// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "DialogueAssetGraph.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueAssetGraph : public UEdGraph
{
	GENERATED_BODY()

public:

	class UDialogueAsset* GetDialogueAsset() const;

	void Clear();
	virtual void RebuildGraph();

	UPROPERTY(Transient)
	TMap<class UDialogueNode*, class UBaseDialogueGraphNode*> DialogueNodeMap;
	
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueAssetGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	//Connection
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;

	//Design
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
};