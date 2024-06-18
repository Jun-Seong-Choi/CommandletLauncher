#pragma once

#include "Framework/Docking/TabManager.h"
#include "Widgets/SCommandletLauncherEditorWidget.h"
#include "CommandletLauncher.h"
#include "CommandletLauncherConfig.h"

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

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost);
	void ExecuteCommandlet(const FString& InCommandletName, const FString& InCommandletArgs);
	void LoadCommandletArguments(const FString& InCommandletName, TArray<FString>& OutCommandletArguments);

public:
	static const FName TabID;

private:
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& InArgs);

	void CreateInternalWidgets();

private:
	TSharedPtr<class FCommandletLauncher> CommandletLauncher = nullptr;
	TSharedPtr<class FCommandletLauncherConfig> CommandletConfig = nullptr;
	TSharedPtr<class SCommandletLauncherEditorWidget> EditorWidget;
};
