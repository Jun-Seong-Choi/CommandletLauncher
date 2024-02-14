#include "Widgets/SCommandletLauncherEditorWidget.h"
#include "Commandlets/Commandlet.h"
#include "SSearchableComboBox.h"
	
#define LOCTEXT_NAMESPACE "CommandletLauncherEditorWidget"

void SCommandletLauncherEditorWidget::Construct(const FArguments& InArgs)
{
	EditorPtr = InArgs._Editor;

	SetCommandlets();

	this->ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Top)
		.Padding(5.0f)
		[
			SNew(STextBlock)
				.Font(FAppStyle::GetFontStyle(TEXT("SmallFont")))
				.Text(LOCTEXT("Commandlet", "Commandlet"))
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Top)
		.Padding(5.0f)
		[
			SNew(SSearchableComboBox)
			.ToolTipText(LOCTEXT("SourceChainOptionsToolTip", "Select source chain to map to this target chain."))
			.OptionsSource(&Commandlets)
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
			{
				return SNew(STextBlock).Text(FText::FromString(*InItem.Get()));
			})
			.OnSelectionChanged_Lambda([this](TSharedPtr<FString> InString, ESelectInfo::Type SelectInfo)
			{
				SelectedCommandlet = InString;						
			})
			[
				SNew(STextBlock)
				.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
				.Text_Lambda([this]()
				{
					if (SelectedCommandlet.IsValid())
					{
						return FText::FromString(*SelectedCommandlet.Get());
					}

					return FText::FromString(*Commandlets[0].Get());
				})
			]
		]
	];
}

void SCommandletLauncherEditorWidget::SetCommandlets()
{	
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		UClass* Class = *ClassIt;
		if (Class->IsChildOf(UCommandlet::StaticClass()) && !Class->HasAnyClassFlags(CLASS_Abstract))
		{
			Commandlets.Add(MakeShared<FString>(Class->GetName()));
		}
	}
}

#undef LOCTEXT_NAMESPACE
