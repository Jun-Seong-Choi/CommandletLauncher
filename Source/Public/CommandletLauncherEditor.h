#pragma once

#include "Framework/Docking/TabManager.h"

static const FName CommandletLauncherEditorTabName("CommandletLauncherEditor");

class FCommandletLauncherEditor : public FAssetEditorToolkit
{
public:

	FCommandletLauncherEditor();
	~FCommandletLauncherEditor();

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	/** IToolkit interface */

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost);

public:

	static const FName TabID;

private:

	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args);

	void CreateInternalWidgets();

private:

	TSharedPtr<SCommandletLauncherEditorWidget> EditorWidget;
};
