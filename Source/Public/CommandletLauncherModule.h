// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "CommandletLauncher.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCommandletLauncherModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	void PluginButtonClicked();

private:
	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<class FCommandletLauncher> CommandletLauncher = nullptr;
};
