// Fill out your copyright notice in the Description page of Project Settings.


#include "SLocalizationSelected.h"

#include "SSimpleButton.h"
#include "SSimpleComboButton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"


void SLocalizationSelected::Construct(const FArguments& InArgs)
{
	PropertyHandle = InArgs._Property;

	OptionTextFilter = MakeShareable(new FOptionTextFilter(FOptionTextFilter::FItemToStringArray::CreateLambda([](const TSharedPtr<FAvailableStringTable>& InItem, OUT TArray< FString >& StringArray) {
	StringArray.Add(InItem->DisplayName.ToString());
	})));

	KeyTextFilter = MakeShareable(new FKeyTextFilter(FKeyTextFilter::FItemToStringArray::CreateLambda([](const TSharedPtr<FString>& InItem, OUT TArray< FString >& StringArray) {
		StringArray.Add(*InItem);
		})));

	OptionsSearchBox.Reset();
	StringTableOptionsCombo.Reset();
	StringTableKeysCombo.Reset();
	
	ChildSlot
	[
		SNew(SSimpleComboButton)
		.Icon(this, &SLocalizationSelected::GetAdvancedTextSettingsComboImage)
		.MenuContent()
		[
			SNew(SBox)
			.WidthOverride(340)
			.Padding(8,5)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(0,5)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text(FText::FromString("IsLocalized"))
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					.HAlign(HAlign_Right)
					[
						SNew(SCheckBox)
						.IsChecked(this, &SLocalizationSelected::GetLocalizableCheckState)
						.OnCheckStateChanged(this, &SLocalizationSelected::HandleLocalizableCheckStateChanged)
					]
				]
				+ SVerticalBox::Slot()
				.Padding(0,5)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(0.6)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text(FText::FromString("String Table"))
					]
					+ SHorizontalBox::Slot()
					.FillWidth(0.5)
					[
						SAssignNew(StringTableOptionsCombo, SComboButton)
							.ComboButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FComboBoxStyle>("ComboBox").ComboButtonStyle)
							.ContentPadding(FMargin(4.0, 2.0))
							.OnGetMenuContent(this, &SLocalizationSelected::OnGetStringTableComboOptions)
							.OnComboBoxOpened(this, &SLocalizationSelected::UpdateStringTableComboOptions)
							.CollapseMenuOnParentFocus(true)
							.ButtonContent()
							[
								SNew(STextBlock)
									.Text(this, &SLocalizationSelected::GetStringTableComboContent)
									.ToolTipText(this, &SLocalizationSelected::GetStringTableComboToolTip)
									//.Font(InArgs._Font)
							]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(0.5)
					.Padding(2.5f,0)
					[
						SAssignNew(StringTableKeysCombo, SComboButton)
							.ComboButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FComboBoxStyle>("ComboBox").ComboButtonStyle)
							.ContentPadding(FMargin(4.0, 2.0))
							.IsEnabled(this, &SLocalizationSelected::IsUnlinkEnabled)
							.OnGetMenuContent(this, &SLocalizationSelected::OnGetStringTableKeyOptions)
							.OnComboBoxOpened(this, &SLocalizationSelected::UpdateStringTableKeyOptions)
							.CollapseMenuOnParentFocus(true)
							.ButtonContent()
							[
								SNew(STextBlock)
									.Text(this, &SLocalizationSelected::GetKeyComboContent)
									.ToolTipText(this, &SLocalizationSelected::GetKeyComboToolTip)
									//.Font(InArgs._Font)
							]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2,0)
					[
						SNew(SSimpleButton)
						.Icon(FAppStyle::GetBrush("Icons.Delete"))
						.IsEnabled(this, &SLocalizationSelected::IsUnlinkEnabled)
						.OnClicked(this, &SLocalizationSelected::OnUnlinkClicked)
					]
				]
			]
		]	
	];
}

const FSlateBrush* SLocalizationSelected::GetAdvancedTextSettingsComboImage() const
{
	if (IsTextLocalizable())
	{
		if (PropertyHandle.IsValid())
		{
			FText TextValue =FText();
			PropertyHandle->GetValue(TextValue);
			if (TextValue.IsFromStringTable())
			{
				return FAppStyle::Get().GetBrush("Icons.SuccessWithColor");
			}
			else
			{
				return FAppStyle::Get().GetBrush("LocalizationDashboard.MenuIcon");
			}
		}
	}
	else
	{
		return FCoreStyle::Get().GetBrush("Icons.Warning");
	}

	return FCoreStyle::Get().GetBrush("Icons.Warning");
}

bool SLocalizationSelected::IsTextLocalizable() const
{
	if (PropertyHandle.IsValid())
	{
		FText TextValue = FText();
		PropertyHandle->GetValue(TextValue);
		return !TextValue.IsCultureInvariant();
	}
	return false;
}

ECheckBoxState SLocalizationSelected::GetLocalizableCheckState() const
{
	if (PropertyHandle.IsValid())
	{
		FText TextValue = FText();
		PropertyHandle->GetValue(TextValue);
		
		if (!TextValue.IsEmpty())
		{	
			const bool bIsLocalized = !TextValue.IsCultureInvariant();
			return bIsLocalized ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;	
		}
	}
	return ECheckBoxState::Unchecked;
}

void SLocalizationSelected::HandleLocalizableCheckStateChanged(ECheckBoxState InCheckboxState)
{
	if (PropertyHandle.IsValid())
	{
		FText NewText = FText();
		FText LocalText = FText();
		PropertyHandle->GetValue(LocalText);

		if (LocalText.IsEmpty())
			return;

		if (InCheckboxState == ECheckBoxState::Checked)
		{
			if (LocalText.IsCultureInvariant())
			{
				// Создание нового локализованного текста из строки
				FString TextContent = LocalText.ToString();

				// Пример генерации ключа
				const FString Namespace = TEXT("DialogSystem");
				const FString Key = FGuid::NewGuid().ToString();

				NewText = FInternationalization::Get().ForUseOnlyByLocMacroAndGraphNodeTextLiterals_CreateText(
					*TextContent,
					*Namespace,
					*Key
				);
			}
		}
		else
		{
			if (!LocalText.IsCultureInvariant())
			{
				NewText = FText::AsCultureInvariant(LocalText.ToString());
			}
		}

		PropertyHandle->SetValue(NewText);
	}
}

TSharedRef<SWidget> SLocalizationSelected::OnGetStringTableComboOptions()
{
	const FComboButtonStyle& ComboButtonStyle = FCoreStyle::Get().GetWidgetStyle< FComboButtonStyle >("ComboButton");
	return SNew(SBorder)
		.BorderImage(&ComboButtonStyle.MenuBorderBrush)
		.Padding(ComboButtonStyle.MenuBorderPadding)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(OptionsSearchBox, SSearchBox)
						.OnTextChanged(this, &SLocalizationSelected::OnOptionsFilterTextChanged)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SWidgetSwitcher)
						.WidgetIndex_Lambda([this]() { return StringTableComboOptions.IsEmpty() ? 0 : 1; })

						+ SWidgetSwitcher::Slot() // Appears when there are no string tables with keys
						.Padding(12)
						[
							SNew(STextBlock).Text(FText::FromString("No string tables available"))
						]

						+ SWidgetSwitcher::Slot() // Appears when there's a string table with at least a key
						[
							SNew(SBox)
								.Padding(4)
								.WidthOverride(280)
								.MaxDesiredHeight(600)
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot()
										.FillHeight(1.f)
										.Padding(0, 5, 0, 0)
										[
											SAssignNew(StringTableOptionsList, SListView<TSharedPtr<FAvailableStringTable>>)
												.ListItemsSource(&StringTableComboOptions)
												.SelectionMode(ESelectionMode::Single)
												.OnGenerateRow(this, &SLocalizationSelected::OnGenerateStringTableComboOption)
												.OnSelectionChanged(this, &SLocalizationSelected::OnStringTableComboChanged)
										]
								]
						]
				]
		];
}

TSharedRef<class ITableRow> SLocalizationSelected::OnGenerateStringTableComboOption(
	TSharedPtr<FAvailableStringTable> InItem, const TSharedRef<class STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
	[
		SNew(STextBlock)
			.Text(InItem->DisplayName)
			.ToolTipText(FText::FromName(InItem->TableId))
	];
}


TSharedRef<SWidget> SLocalizationSelected::OnGetStringTableKeyOptions()
{
	const FComboButtonStyle& ComboButtonStyle = FCoreStyle::Get().GetWidgetStyle< FComboButtonStyle >("ComboButton");
	return SNew(SBorder)
		.BorderImage(&ComboButtonStyle.MenuBorderBrush)
		.Padding(ComboButtonStyle.MenuBorderPadding)
		[
			SNew(SBox)
				.Padding(4)
				.WidthOverride(280)
				.MaxDesiredHeight(600)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(KeysSearchBox, SSearchBox)
								.OnTextChanged(this, &SLocalizationSelected::OnKeysFilterTextChanged)
						]
						+ SVerticalBox::Slot()
						.FillHeight(1.f)
						.Padding(0, 5, 0, 0)
						[
							SAssignNew(StringTableKeysList, SListView<TSharedPtr<FString>>)
								.ListItemsSource(&KeyComboOptions)
								.SelectionMode(ESelectionMode::Single)
								.OnGenerateRow(this, &SLocalizationSelected::OnGenerateStringTableKeyOption)
								.OnSelectionChanged(this, &SLocalizationSelected::OnKeyComboChanged)
						]
				]
		];
}


TSharedRef<class ITableRow> SLocalizationSelected::OnGenerateStringTableKeyOption(TSharedPtr<FString> InItem,
                                                                                  const TSharedRef<class STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(STextBlock)
				.Text(FText::FromString(*InItem))
				.ToolTipText(FText::FromString(*InItem))
		];
}

void SLocalizationSelected::OnStringTableComboChanged(TSharedPtr<FAvailableStringTable> NewSelection,
	ESelectInfo::Type SelectInfo)
{
	if (SelectInfo != ESelectInfo::Direct && NewSelection.IsValid())
	{
		// Make sure any selected string table asset is loaded
		FName TableId = NewSelection->TableId;
		IStringTableEngineBridge::FullyLoadStringTableAsset(TableId);

		FStringTableConstPtr StringTable = FStringTableRegistry::Get().FindStringTable(TableId);
		if (StringTable.IsValid())
		{
			// Just use the first key when changing the string table
			StringTable->EnumerateSourceStrings([&](const FString& InKey, const FString& InSourceString) -> bool
				{
					SetTableIdAndKey(TableId, InKey);
					return false; // stop enumeration
				});

			StringTableOptionsCombo->SetIsOpen(false);

			OptionsSearchBox->SetText(FText::GetEmpty());
		}
	}
}

void SLocalizationSelected::UpdateStringTableComboOptions()
{
	FName CurrentTableId;
	{
		FString TmpKey;
		GetTableIdAndKey(CurrentTableId, TmpKey);
	}

	TSharedPtr<FAvailableStringTable> SelectedStringTableComboEntry;
	StringTableComboOptions.Reset();

	// Process assets first (as they may currently be unloaded)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

		TArray<FAssetData> StringTableAssets;
		AssetRegistryModule.Get().GetAssetsByClass(UStringTable::StaticClass()->GetClassPathName(), StringTableAssets);

		for (const FAssetData& StringTableAsset : StringTableAssets)
		{
			FName StringTableId = *StringTableAsset.GetObjectPathString();
			// Only allow string tables assets that have entries to be visible otherwise unexpected behavior happens for the user
			bool HasEntries = false;
			FStringTableConstPtr StringTable = FStringTableRegistry::Get().FindStringTable(StringTableId);
			if (StringTable.IsValid())
			{
				StringTable->EnumerateSourceStrings([&](const FString& InKey, const FString& InSourceString) -> bool
					{
						HasEntries = true;
						return false; // stop enumeration
					});
			}
			else
			{
				// Asset is currently unloaded, so just assume it has entries
				HasEntries = true;
			}

			if (!HasEntries)
			{
				continue; // continue on to the next string table asset
			}

			TSharedRef<FAvailableStringTable> AvailableStringTableEntry = MakeShared<FAvailableStringTable>();
			AvailableStringTableEntry->TableId = StringTableId;
			AvailableStringTableEntry->DisplayName = FText::FromName(StringTableAsset.AssetName);
			if (StringTableId == CurrentTableId)
			{
				SelectedStringTableComboEntry = AvailableStringTableEntry;
			}
			if (OptionTextFilter->PassesFilter(AvailableStringTableEntry))
			{
				StringTableComboOptions.Add(AvailableStringTableEntry);
			}
		}
	}

	// Process the remaining non-asset string tables now
	FStringTableRegistry::Get().EnumerateStringTables([&](const FName& InTableId, const FStringTableConstRef& InStringTable) -> bool
		{
			const bool bAlreadyAdded = StringTableComboOptions.ContainsByPredicate([InTableId](const TSharedPtr<FAvailableStringTable>& InAvailableStringTable)
				{
					return InAvailableStringTable->TableId == InTableId;
				});

			bool bHasEntries = false;
			InStringTable->EnumerateSourceStrings([&bHasEntries](const FString& InKey, const FString& InSourceString) -> bool
				{
					bHasEntries = true;
					return false; // stop enumeration
				});

			if (!bAlreadyAdded && bHasEntries)
			{
				TSharedRef<FAvailableStringTable> AvailableStringTableEntry = MakeShared<FAvailableStringTable>();
				AvailableStringTableEntry->TableId = InTableId;
				AvailableStringTableEntry->DisplayName = FText::FromName(InTableId);
				if (InTableId == CurrentTableId)
				{
					SelectedStringTableComboEntry = AvailableStringTableEntry;
				}
				if (OptionTextFilter->PassesFilter(AvailableStringTableEntry))
				{
					StringTableComboOptions.Add(AvailableStringTableEntry);
				}
			}

			return true; // continue enumeration
		});

	StringTableComboOptions.Sort([](const TSharedPtr<FAvailableStringTable>& InOne, const TSharedPtr<FAvailableStringTable>& InTwo)
		{
			return InOne->DisplayName.ToString() < InTwo->DisplayName.ToString();
		});

	StringTableOptionsList->RebuildList();

	if (SelectedStringTableComboEntry.IsValid())
	{
		StringTableOptionsList->SetItemSelection(SelectedStringTableComboEntry, true);
	}
	else
	{
		StringTableOptionsList->ClearSelection();
	}
}

void SLocalizationSelected::OnKeyComboChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// If it's set from code, we did that on purpose
	if (SelectInfo != ESelectInfo::Direct && NewSelection.IsValid())
	{
		FName CurrentTableId;
		{
			FString TmpKey;
			GetTableIdAndKey(CurrentTableId, TmpKey);
		}

		SetTableIdAndKey(CurrentTableId, *NewSelection);

		StringTableKeysCombo->SetIsOpen(false);

		KeysSearchBox->SetText(FText::GetEmpty());
	}
}

void SLocalizationSelected::UpdateStringTableKeyOptions()
{
	FName CurrentTableId;
	FString CurrentKey;
	GetTableIdAndKey(CurrentTableId, CurrentKey);

	TSharedPtr<FString> SelectedKeyComboEntry;
	KeyComboOptions.Reset();

	if (!CurrentTableId.IsNone())
	{
		FStringTableConstPtr StringTable = FStringTableRegistry::Get().FindStringTable(CurrentTableId);
		if (StringTable.IsValid())
		{
			StringTable->EnumerateSourceStrings([&](const FString& InKey, const FString& InSourceString) -> bool
				{
					TSharedRef<FString> KeyComboEntry = MakeShared<FString>(InKey);
					if (InKey.Equals(CurrentKey, ESearchCase::CaseSensitive))
					{
						SelectedKeyComboEntry = KeyComboEntry;
					}
					if (KeyTextFilter->PassesFilter(KeyComboEntry))
					{
						KeyComboOptions.Add(KeyComboEntry);
					}
					return true; // continue enumeration
				});
		}
	}

	KeyComboOptions.Sort([](const TSharedPtr<FString>& InOne, const TSharedPtr<FString>& InTwo)
		{
			return *InOne < *InTwo;
		});

	StringTableKeysList->RebuildList();

	if (SelectedKeyComboEntry.IsValid())
	{
		StringTableKeysList->SetItemSelection(SelectedKeyComboEntry, true);
	}
	else
	{
		StringTableKeysList->ClearSelection();
	}
}

void SLocalizationSelected::OnOptionsFilterTextChanged(const FText& InNewText)
{
	OptionTextFilter->SetRawFilterText(InNewText);
	OptionsSearchBox->SetError(OptionTextFilter->GetFilterErrorText());

	UpdateStringTableComboOptions();
}

void SLocalizationSelected::OnKeysFilterTextChanged(const FText& InNewText)
{
	KeyTextFilter->SetRawFilterText(InNewText);
	KeysSearchBox->SetError(KeyTextFilter->GetFilterErrorText());

	UpdateStringTableKeyOptions();
}

void SLocalizationSelected::GetTableIdAndKey(FName& OutTableId, FString& OutKey) const
{
	if (PropertyHandle.IsValid())
	{
		FText PropertyValue = FText();
		PropertyHandle->GetValue(PropertyValue);
		FTextInspector::GetTableIdAndKey(PropertyValue, OutTableId, OutKey);

		// Verify that all texts are using the same string table and key
	
		FName TmpTableId;
		FString TmpKey;
		if (FTextInspector::GetTableIdAndKey(PropertyValue, TmpTableId, TmpKey) && OutTableId == TmpTableId)
		{
			if (!OutKey.Equals(TmpKey, ESearchCase::CaseSensitive))
			{
				// Not using the same key - clear the key but keep the table and keep enumerating to verify the table on the remaining texts
				OutKey.Reset();
			}
		}
		else
		{
			// Not using a string table, or using a different string table - clear both table ID and key
			OutTableId = FName();
			OutKey.Reset();
		}
	}
}

void SLocalizationSelected::SetTableIdAndKey(const FName InTableId, const FString& InKey)
{
	if (PropertyHandle.IsValid())
	{
		const FText TextToSet = FText::FromStringTable(InTableId, InKey);
		if (TextToSet.IsFromStringTable())
		{
			if (!TextToSet.IsEmpty())
			{
				PropertyHandle->SetValue(TextToSet);
			}
		}
	}
}

FText SLocalizationSelected::GetStringTableComboContent() const
{
	FName CurrentTableId;
	{
		FString TmpKey;
		GetTableIdAndKey(CurrentTableId, TmpKey);
	}

	return FText::FromString(FPackageName::GetLongPackageAssetName(CurrentTableId.ToString()));
}

FText SLocalizationSelected::GetStringTableComboToolTip() const
{
	FName CurrentTableId;
	{
		FString TmpKey;
		GetTableIdAndKey(CurrentTableId, TmpKey);
	}

	return FText::FromName(CurrentTableId);
}

FText SLocalizationSelected::GetKeyComboContent() const
{
	FString CurrentKey;
	{
		FName TmpTableId;
		GetTableIdAndKey(TmpTableId, CurrentKey);
	}

	if (CurrentKey.IsEmpty())
	{
		return FText::FromString("No Key");
	}

	return FText::FromString(MoveTemp(CurrentKey));
}

FText SLocalizationSelected::GetKeyComboToolTip() const
{
	return GetKeyComboContent();
}

bool SLocalizationSelected::IsUnlinkEnabled() const
{
	if (PropertyHandle.IsValid())
	{
		FText LocalText = FText();
		PropertyHandle->GetValue(LocalText);
		if (LocalText.IsFromStringTable())
		{
			return true;
		}
	}
	
	return false;
}

FReply SLocalizationSelected::OnUnlinkClicked()
{
	if (PropertyHandle.IsValid())
	{
		FText CurrentText = FText();
		PropertyHandle->GetValue(CurrentText);
		if (CurrentText.IsFromStringTable())
		{
			// Make a copy of the FText separate from the string table but generate a new stable namespace and key
			// This prevents problems with properties that disallow empty text (e.g. enum display name)
			FString NewNamespace;
			FString NewKey;
			/*EditableTextProperty->GetStableTextId(
				TextIndex,
				IEditableTextProperty::ETextPropertyEditAction::EditedKey,
				CurrentText.ToString(),
				FString(),
				FString(),
				NewNamespace,
				NewKey
			);*/

			FText TextToSet = FText::ChangeKey(NewNamespace, NewKey, CurrentText);
			if (!TextToSet.IsEmpty())
			{
				PropertyHandle->SetValue(TextToSet);
			}

		}
	}
	return FReply::Handled();
}
