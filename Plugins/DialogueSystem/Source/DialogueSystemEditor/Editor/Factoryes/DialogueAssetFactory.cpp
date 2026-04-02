// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueAssetFactory.h"


UDialogueAssetFactory::UDialogueAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UDialogueAsset::StaticClass();
}


UObject* UDialogueAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UDialogueAsset* NewDialogueAsset = NewObject<UDialogueAsset>(InParent, Class, Name, Flags | RF_Transactional);
	return NewDialogueAsset;
}

uint32 UDialogueAssetFactory::GetMenuCategories() const
{
	return FDialogueSystemEditor::DialogueAssetCategory;
}

FText UDialogueAssetFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Dialogue"));
}

FString UDialogueAssetFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("New Dialogue"));
}
