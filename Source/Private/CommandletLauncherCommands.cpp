// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommandletLauncherCommands.h"

#define LOCTEXT_NAMESPACE "FCommandletLauncherModule"

void FCommandletLauncherCommands::RegisterCommands()
{
	UI_COMMAND(CommandletLauncherAction, "CommandletLauncher", "Execute CommandletLauncher action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
