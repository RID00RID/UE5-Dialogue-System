// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Misc/TextFilter.h"

/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API SLocalizationSelected : public SCompoundWidget
{

	struct FAvailableStringTable
	{
		FName TableId;
		FText DisplayName;
	};
	
public:
	


	SLATE_BEGIN_ARGS(SLocalizationSelected) 
	: _Property()
	{
			
	}
		SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, Property)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	const FSlateBrush* GetAdvancedTextSettingsComboImage() const;


#pragma region Localization

protected:

	bool IsTextLocalizable() const;
	ECheckBoxState GetLocalizableCheckState() const;
	void HandleLocalizableCheckStateChanged(ECheckBoxState InCheckboxState);

	TSharedRef<SWidget> OnGetStringTableComboOptions();
	TSharedRef<class ITableRow> OnGenerateStringTableComboOption(TSharedPtr<FAvailableStringTable> InItem, const TSharedRef<class STableViewBase>& OwnerTable);

	TSharedRef<SWidget> OnGetStringTableKeyOptions();
	TSharedRef<class ITableRow> OnGenerateStringTableKeyOption(TSharedPtr<FString> InItem, const TSharedRef<class STableViewBase>& OwnerTable);

	void OnStringTableComboChanged(TSharedPtr<FAvailableStringTable> NewSelection, ESelectInfo::Type SelectInfo);
	void UpdateStringTableComboOptions();

	void OnKeyComboChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void UpdateStringTableKeyOptions();


	void OnOptionsFilterTextChanged(const FText& InNewText);
	void OnKeysFilterTextChanged(const FText& InNewText);

	void GetTableIdAndKey(FName& OutTableId, FString& OutKey) const;
	void SetTableIdAndKey(const FName InTableId, const FString& InKey);

	FText GetStringTableComboContent() const;
	FText GetStringTableComboToolTip() const;

	FText GetKeyComboContent() const;
	FText GetKeyComboToolTip() const;


	bool IsUnlinkEnabled() const;
	FReply OnUnlinkClicked();

	
	
#pragma endregion 
	
private:

	TSharedPtr<IPropertyHandle> PropertyHandle;

	
	using FOptionTextFilter = TTextFilter<TSharedPtr<FAvailableStringTable>>;
	TSharedPtr<FOptionTextFilter> OptionTextFilter;
	TSharedPtr<SSearchBox> OptionsSearchBox;

	TSharedPtr<SComboButton> StringTableOptionsCombo;
	TSharedPtr<SListView<TSharedPtr<FAvailableStringTable>>> StringTableOptionsList;
	TArray<TSharedPtr<FAvailableStringTable>> StringTableComboOptions;


	using FKeyTextFilter = TTextFilter< TSharedPtr<FString> >;
	TSharedPtr<FKeyTextFilter> KeyTextFilter;
	TSharedPtr<SSearchBox> KeysSearchBox;


	TSharedPtr<SComboButton> StringTableKeysCombo;
	TSharedPtr<SListView<TSharedPtr<FString>>> StringTableKeysList;
	TArray<TSharedPtr<FString>> KeyComboOptions;
};
