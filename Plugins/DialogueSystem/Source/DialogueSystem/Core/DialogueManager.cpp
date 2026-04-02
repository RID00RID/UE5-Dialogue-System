// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager.h"


// Sets default values for this component's properties
UDialogueManager::UDialogueManager(): CurrentDialogue(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UDialogueManager::BeginPlay()
{
	Super::BeginPlay();

}

UDialogueManager* UDialogueManager::GetDialogueManager(AActor* FromActor)
{
	if (!FromActor) return nullptr;
	return Cast<UDialogueManager>(FromActor->GetComponentByClass(UDialogueManager::StaticClass()));
}


#pragma region Dialogue

bool UDialogueManager::OpenDialogueAsset(UDialogueAsset* NewDialogue, FName StartDialogueNode)
{
	if (!NewDialogue) return false;
	if (IsValid(CurrentDialogue))
	{
		CurrentDialogue->MarkAsGarbage();
		CurrentDialogue->DialogueManager = nullptr;
		CurrentDialogue = nullptr;
	}
	
	CurrentDialogue = NewObject<UDialogueAsset>(GetOwner(), FName(NewDialogue->GetName()), RF_NoFlags, NewDialogue);
	if (!CurrentDialogue) return false;
	CurrentDialogue->InitDialogueAsset(this, StartDialogueNode);
	OnOpenDialogue.Broadcast(CurrentDialogue);
	return true;

}

FDialogueNodeData UDialogueManager::GetCurrentDialogueNode() const
{
	if (IsValid(CurrentDialogue))
	{
		return CurrentDialogue->GetCurrentDialogueNode();
	}
	return FDialogueNodeData();
}

bool UDialogueManager::NextDialogNode()
{
	if (CurrentDialogue)
	{
		FDialogueNodeData Data = FDialogueNodeData();
		bool Result = CurrentDialogue->GetNextNode(Data);
		OnChangeDialogue.Broadcast(Data);
		return Result;
	}
	return false;
}

bool UDialogueManager::ChangeNextDialogNode(FDialogueResponse Response)
{
	if (CurrentDialogue && Response.NextNode.IsValid())
	{
		FDialogueNodeData Data = FDialogueNodeData();
		bool Result = CurrentDialogue->ChangeNextNode(Response.NextNode, Data);
		OnChangeDialogue.Broadcast(Data);
		return Result;
	}
	return false;
}

void UDialogueManager::CloseDialogue()
{
	if (CurrentDialogue)
	{
		CurrentDialogue->MarkAsGarbage();
		CurrentDialogue = nullptr;
	}
	OnCloseDialogue.Broadcast();
}


#pragma endregion 
