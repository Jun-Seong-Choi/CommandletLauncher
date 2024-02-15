// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommandletLauncherModule.h"
#include "CommandletLauncherStyle.h"
#include "CommandletLauncherCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "UObject/Object.h"
#include "CommandletLauncherEditor.h"

static const FName CommandletLauncherName("CommandletLauncher");

#define LOCTEXT_NAMESPACE "FCommandletLauncherModule"

void FCommandletLauncherModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FCommandletLauncherStyle::Initialize();
    FCommandletLauncherStyle::ReloadTextures();

    FCommandletLauncherCommands::Register();

    PluginCommands = MakeShareable(new FUICommandList);

    PluginCommands->MapAction(
        FCommandletLauncherCommands::Get().CommandletLauncherAction,
        FExecuteAction::CreateRaw(this, &FCommandletLauncherModule::PluginButtonClicked),
        FCanExecuteAction());

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCommandletLauncherModule::RegisterMenus));
}

void FCommandletLauncherModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.

    UToolMenus::UnRegisterStartupCallback(this);

    UToolMenus::UnregisterOwner(this);

    FCommandletLauncherStyle::Shutdown();

    FCommandletLauncherCommands::Unregister();
}

void FCommandletLauncherModule::PluginButtonClicked()
{
    // Put your "OnButtonClicked" stuff here

    TSharedRef<FCommandletLauncherEditor> Editor = MakeShared<FCommandletLauncherEditor>();
    Editor->InitEditor(EToolkitMode::Standalone, nullptr);
}

void FCommandletLauncherModule::RegisterMenus()
{
    // Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
    FToolMenuOwnerScoped OwnerScoped(this);

    {
        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
        {
            FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
            Section.AddMenuEntryWithCommandList(FCommandletLauncherCommands::Get().CommandletLauncherAction, PluginCommands);
        }
    }

    {
        UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
        {
            FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
            {
                FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCommandletLauncherCommands::Get().CommandletLauncherAction));
                Entry.SetCommandList(PluginCommands);
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommandletLauncherModule, CommandletLauncher)
