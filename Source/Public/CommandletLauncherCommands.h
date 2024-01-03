// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CommandletLauncherStyle.h"

class FCommandletLauncherCommands : public TCommands<FCommandletLauncherCommands>
{
public:

	FCommandletLauncherCommands()
		: TCommands<FCommandletLauncherCommands>(TEXT("CommandletLauncher"), NSLOCTEXT("Contexts", "CommandletLauncher", "CommandletLauncher Plugin"), NAME_None, FCommandletLauncherStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > CommandletLauncherAction;
};
