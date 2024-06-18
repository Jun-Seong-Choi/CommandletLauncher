#pragma once

#include "CoreMinimal.h"
#include "CommandletLauncherEditor.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/STextComboBox.h"

class SCommandletLauncherEditorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCommandletLauncherEditorWidget) {}
		SLATE_ARGUMENT(TWeakPtr<class FCommandletLauncherEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetCommandlets();

private:
	void RefreshArguments(const FString& InCommandletName);

private:
	TWeakPtr<class FCommandletLauncherEditor> EditorPtr;

	TSharedPtr<SEditableTextBox> EditableText;
	TSharedPtr<STextComboBox> ComboBox;

	TArray<TSharedPtr<FString>> Commandlets;
	TArray<TSharedPtr<FString>> Arguments;
	TSharedPtr<FString> SelectedCommandlet;
	TSharedPtr<FString> SelectedArgument;
};
