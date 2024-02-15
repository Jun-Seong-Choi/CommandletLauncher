#pragma once

#include "Framework/Docking/TabManager.h"
#include "Widgets/SCommandletLauncherEditorWidget.h"
#include "CommandletLauncher.h"

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
	void ExecuteCommandlet(const FString& InCommandletName, const FString& InCommandletArgs);

public:
	static const FName TabID;

private:
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& InArgs);

	void CreateInternalWidgets();

private:
	TSharedPtr<FCommandletLauncher> CommandletLauncher = nullptr;
	TSharedPtr<SCommandletLauncherEditorWidget> EditorWidget;
};
