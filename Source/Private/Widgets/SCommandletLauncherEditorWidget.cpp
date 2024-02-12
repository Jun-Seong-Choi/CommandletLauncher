#include "Widgets/SCommandletLauncherEditorWidget.h"

void SCommandletLauncherEditorWidget::Construct(const FArguments& InArgs, TWeakPtr<FCommandletLauncherEditor> InEditorPtr)
{
	EditorPtr = InEditorPtr;
}
