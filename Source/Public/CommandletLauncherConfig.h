#pragma once

#include "Dom/JsonObject.h"

class FCommandletLauncherConfig
{
public:
	FCommandletLauncherConfig();
	~FCommandletLauncherConfig();

	void SaveCommandLineToConfig(const FString& InCommandletName, const FString& InCommandletArgs);
	const TArray<FString> LoadCommandLineFromConfig(const FString& InCommandletName);

private:
	bool SaveJsonFile(const FString& InFileName, TSharedPtr<FJsonObject> JsonObject);
	TSharedPtr<FJsonObject> LoadJsonFile(const FString& InFileName);
	const FString GetConfigFileName(const FString& InCommandletName);
};