
#include "FDialogueEditorStyle.h"


#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FDialogueEditorStyle::StyleSet = nullptr;

FName FDialogueEditorStyle::GetStyleSetName()
{
	static FName FlowEditorStyleName(TEXT("DialogueEditorStyle"));
	return FlowEditorStyleName;
}

void FDialogueEditorStyle::Initialize()
{
	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("DialogueEditorStyle")));

	const FVector2D Icon16(16.0f, 16.0f);
	const FVector2D Icon20(20.0f, 20.0f);
	const FVector2D Icon30(30.0f, 30.0f);
	const FVector2D Icon40(40.0f, 40.0f);
	const FVector2D Icon64(64.0f, 64.0f);

	
	// Flow assets
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("DialogueSystem"))->GetBaseDir() / TEXT("Resources"));

	StyleSet->Set("DialogueEditor.Graph.DialogNodeBody",    new BOX_BRUSH("DialogNode_Body",    FMargin(16.f / 64.f)));
	StyleSet->Set("DialogueEditor.Graph.DialogNodeContent", new BOX_BRUSH("DialogNode_Content", FMargin(16.f / 64.f)));
	StyleSet->Set("DialogueEditor.Graph.NodeHighlight",     new BOX_BRUSH("NodeHighlight",      FMargin(16.f / 64.f)));
	StyleSet->Set("DialogueEditor.Graph.FieldBody",         new BOX_BRUSH("FieldBody",          FMargin(16.f / 64.f)));

	StyleSet->Set("DialogueEditor.Icon.StartDialog",    new IMAGE_BRUSH(TEXT("StartDialogIcon"), Icon64));
	StyleSet->Set("DialogueEditor.Icon.EndDialog",      new IMAGE_BRUSH(TEXT("EndDialogIcon"),   Icon64));
	StyleSet->Set("DialogueEditor.Icon.Dialogue",       new IMAGE_BRUSH(TEXT("DialogIcon"),      Icon64));
	StyleSet->Set("DialogueEditor.Icon.ErrorDialog",    new IMAGE_BRUSH(TEXT("ErrorIcon"),       Icon64));
	StyleSet->Set("DialogueEditor.Icon.DialogueChoice", new IMAGE_BRUSH(TEXT("ChoiceIcon"),      Icon64));
	StyleSet->Set("DialogueEditor.Icon.EventDialog",    new IMAGE_BRUSH(TEXT("EventDialogIcon"), Icon64));
	
	StyleSet->Set("DialogueEditor.Icon.Condition",    new IMAGE_BRUSH(TEXT("EventDialogIcon"), Icon16));
	StyleSet->Set("DialogueEditor.Icon.ConditionActive",         new IMAGE_BRUSH(TEXT("ConditionIcon"),   Icon16));


	{
		FSlateBrush NormalBrush = *FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody");
		NormalBrush.TintColor = FLinearColor::White;

		FSlateBrush HoverBrush = *FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody");
		HoverBrush.TintColor = FLinearColor::Gray;
		
		{
			const FButtonStyle DialogButton = FButtonStyle()
			.SetNormal(NormalBrush)
			.SetHovered(HoverBrush)
			.SetPressed(HoverBrush);
			StyleSet->Set(FName("DialogButton"), DialogButton);
		};
	}


	{
		FSlateBrush NormalBrush = *FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody");
		NormalBrush.TintColor = FLinearColor::Transparent;

		FSlateBrush HoverBrush = *FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody");
		HoverBrush.TintColor = FLinearColor::White;

		FSlateBrush PressedBrush = *FDialogueEditorStyle::GetBrush("DialogueEditor.Graph.FieldBody");
		HoverBrush.TintColor = FLinearColor::Gray;
		
		{
			const FButtonStyle RemoveButton = FButtonStyle()
			.SetNormal(NormalBrush)
			.SetHovered(HoverBrush)
			.SetPressed(PressedBrush);
			StyleSet->Set(FName("RemoveButton"), RemoveButton);
		};
	}
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

void FDialogueEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
	ensure(StyleSet.IsUnique());
	StyleSet.Reset();
}

#undef BORDER_BRUSH
#undef BOX_BRUSH
#undef IMAGE_BRUSH
#undef IMAGE_BRUSH_SVG