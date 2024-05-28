#pragma once

#include "CoreMinimal.h"
#include "CommandletLauncherEditor.h"
#include "Widgets/SCompoundWidget.h"

class SCommandletLauncherEditorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCommandletLauncherEditorWidget) {}
		SLATE_ARGUMENT(TWeakPtr<class FCommandletLauncherEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetCommandlets();

	void OnCommandletCommitted(const FText& InNewText, ETextCommit::Type InCommitType);

private:

	TWeakPtr<class FCommandletLauncherEditor> EditorPtr;

	TSharedPtr<SEditableTextBox> EditableText;

	TArray<TSharedPtr<FString>> Commandlets;
	TArray<TSharedPtr<FString>> Arguments;
	TSharedPtr<FString> SelectedCommandlet;
	TSharedPtr<FString> SelectedArgument;
};
