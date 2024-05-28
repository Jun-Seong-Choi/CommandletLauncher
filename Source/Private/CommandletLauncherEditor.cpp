#include "CommandletLauncherEditor.h"
#include "Framework/Docking/TabManager.h"

#define LOCTEXT_NAMESPACE "CommandletLauncherEditor"

const FName FCommandletLauncherEditor::TabID(TEXT("CommandletLauncherEditor"));

FCommandletLauncherEditor::FCommandletLauncherEditor()
{
}

FCommandletLauncherEditor::~FCommandletLauncherEditor()
{
}

FName FCommandletLauncherEditor::GetToolkitFName() const
{
	return FName(TEXT("CommandletLauncherEditor"));
}

FText FCommandletLauncherEditor::GetBaseToolkitName() const
{
	return FText::FromString(FString(TEXT("CommandletLauncherEditor")));
}

FString FCommandletLauncherEditor::GetWorldCentricTabPrefix() const
{
	return FString(TEXT("CommandletLauncherEditor"));
}

FLinearColor FCommandletLauncherEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

void FCommandletLauncherEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_CommandletLauncherEditor", "Commandlet Launcher Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(TabID, FOnSpawnTab::CreateSP(this, &FCommandletLauncherEditor::SpawnTab))
		.SetDisplayName(LOCTEXT("CommandletLauncherTab", "CommandletLauncher"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Info"));
}

void FCommandletLauncherEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(TabID);
}

void FCommandletLauncherEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CommandletLauncher_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(TabID, ETabState::OpenedTab)
			)
		);

	CreateInternalWidgets();

	USkeletalMeshComponent* Object = NewObject<USkeletalMeshComponent>();

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, CommandletLauncherEditorTabName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, Object);
}

void FCommandletLauncherEditor::ExecuteCommandlet(const FString& InCommandletName, const FString& InCommandletArgs)
{
	if (!CommandletLauncher.IsValid())
	{
		CommandletLauncher = MakeShared<FCommandletLauncher>();
	}

	CommandletLauncher->CreateNewCommandletProcess(InCommandletName, InCommandletArgs);
}

TSharedRef<SDockTab> FCommandletLauncherEditor::SpawnTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		.Label(LOCTEXT("CommandletLauncherEditor", "CommandletLauncher"))
		[
			EditorWidget.ToSharedRef()
		];
}

void FCommandletLauncherEditor::CreateInternalWidgets()
{
	EditorWidget = SNew(SCommandletLauncherEditorWidget)
		.Editor(SharedThis(this));
}

#undef LOCTEXT_NAMESPACE
