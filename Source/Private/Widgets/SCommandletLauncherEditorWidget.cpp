#include "Widgets/SCommandletLauncherEditorWidget.h"
#include "Commandlets/Commandlet.h"
#include "SSearchableComboBox.h"
#include "Widgets/Input/STextComboBox.h"
	
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
			.Padding(5.0f)
			[
				SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.0f, 5.0f)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
							.Font(FAppStyle::GetFontStyle(TEXT("SmallFont")))
							.Text(LOCTEXT("CommandletLauncherText", "-RUN="))
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.0f)
					.VAlign(VAlign_Center)
					[
						SNew(SSearchableComboBox)
							.ToolTipText(LOCTEXT("CommandlettLauncherToolTip", "Select the commandlet to run."))
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

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.0f)
					[
						SAssignNew(EditableText, SEditableTextBox)
							.MinDesiredWidth(500)
							.Font(FAppStyle::GetFontStyle(TEXT("SmallFont")))
							.OnTextCommitted_Lambda([this](const FText& InNewText, ETextCommit::Type InCommitType)
							{
								SelectedArgument = MakeShareable(new FString(InNewText.ToString()));
							})
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(STextComboBox)
							.Font(FAppStyle::GetFontStyle(TEXT("SmallFont")))
							.OptionsSource(&Arguments)
							.OnSelectionChanged_Lambda([&](TSharedPtr<FString> InString, ESelectInfo::Type)
							{
								SelectedArgument = InString;
								EditableText->SetText(FText::FromString(*SelectedArgument));
							})
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(SButton)
							.ButtonStyle(FAppStyle::Get(), "FlatButton")
							.ForegroundColor(FLinearColor::White)
							.ContentPadding(FMargin(6, 2))
							.Text(LOCTEXT("CommandletLauncherExecutor", "Execute"))
							.OnClicked_Lambda([this]()
							{
								if (EditorPtr.IsValid() && !SelectedCommandlet->IsEmpty())
								{
									EditorPtr.Pin()->ExecuteCommandlet(*SelectedCommandlet.Get(), *SelectedArgument.Get());
								}
								
								return FReply::Handled();
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

	Arguments.Add(MakeShareable(new FString(TEXT("Test-Name"))));
}

#undef LOCTEXT_NAMESPACE
