// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommandletLauncherStyle.h"
#include "CommandletLauncher.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCommandletLauncherStyle::StyleInstance = nullptr;

void FCommandletLauncherStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCommandletLauncherStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCommandletLauncherStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CommandletLauncherStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FCommandletLauncherStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CommandletLauncherStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CommandletLauncher")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CommandletLauncher.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FCommandletLauncherStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCommandletLauncherStyle::Get()
{
	return *StyleInstance;
}
