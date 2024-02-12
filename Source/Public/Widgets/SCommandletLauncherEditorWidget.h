#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/STextComboBox.h"

class FCommandletLauncherEditor;

class SCommandletLauncherEditorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCommandletLauncherEditorWidget) {}
		SLATE_ARGUMENT(TWeakPtr<FCommandletLauncherEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TWeakPtr<FCommandletLauncherEditor> EditorPtr;
};

