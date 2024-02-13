#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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

