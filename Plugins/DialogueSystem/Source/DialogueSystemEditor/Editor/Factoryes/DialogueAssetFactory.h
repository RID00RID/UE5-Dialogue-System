// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DialogueSystem/Core/DialogueAsset.h"

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetTypeActions_Base.h"
#include "DialogueSystemEditor.h"
#include "DialogueSystemEditor/Editor/DialogueAssetEditor.h"
#include "DialogueAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	UDialogueAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;
	
};

class FAssetTypeActions_DialogueAssetAction : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return FText::FromString(TEXT("Dialogue Asset")); }
	virtual FColor GetTypeColor() const override { return FColor::FromHex("C91D55"); }
	virtual UClass* GetSupportedClass() const override { return UDialogueAsset::StaticClass(); }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override
	{
		const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

		for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
		{
			if (UDialogueAsset* DialogueAsset = Cast<UDialogueAsset>(*ObjIt))
			{
				TSharedRef<FDialogueAssetEditor> NewUITreeEditor(new FDialogueAssetEditor());
				NewUITreeEditor->InitDialogueAssetEditor(Mode, EditWithinLevelEditor, DialogueAsset);
			}
		}
	}
	virtual uint32 GetCategories() override { return FDialogueSystemEditor::DialogueAssetCategory; }
};

