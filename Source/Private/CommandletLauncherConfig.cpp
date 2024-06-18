#include "CommandletLauncherConfig.h"
#include "HAL/FileManager.h"
#include "JsonObjectConverter.h"

static const FString JsonFieldName_Values("Values");

FCommandletLauncherConfig::FCommandletLauncherConfig()
{
}

FCommandletLauncherConfig::~FCommandletLauncherConfig()
{
}

bool FCommandletLauncherConfig::SaveJsonFile(const FString& InFileName, TSharedPtr<FJsonObject> JsonObject)
{
	FString NewContents;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&NewContents);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		return FFileHelper::SaveStringToFile(NewContents, *InFileName);
	}

	return false;
}

TSharedPtr<FJsonObject> FCommandletLauncherConfig::LoadJsonFile(const FString& InFileName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	FString Contents;
	if (FFileHelper::LoadFileToString(Contents, *InFileName))
	{
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Contents);
		FJsonSerializer::Deserialize(Reader, JsonObject);
	}

	return JsonObject;
}

void FCommandletLauncherConfig::SaveCommandLineToConfig(const FString& InCommandletName, const FString& InCommandletArgs)
{
	TArray<FString> StrValues;

	const FString JsonFileName = GetConfigFileName(InCommandletName);
	TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(JsonFileName);
	if (JsonObject.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>> OldJsonValues = JsonObject->GetArrayField(JsonFieldName_Values);
		for (const auto& Value : OldJsonValues)
		{
			StrValues.AddUnique(Value->AsString());
		}
	}

	StrValues.AddUnique(InCommandletArgs);

	TArray<TSharedPtr<FJsonValue>> NewJsonValues;
	for (const auto& Str : StrValues)
	{
		TSharedRef<FJsonValueString> JsonValue = MakeShareable(new FJsonValueString(*Str));
		NewJsonValues.Add(JsonValue);
	}

	JsonObject->SetArrayField(JsonFieldName_Values, NewJsonValues);
	SaveJsonFile(JsonFileName, JsonObject);
}

const TArray<FString> FCommandletLauncherConfig::LoadCommandLineFromConfig(const FString& InCommandletName)
{
	TArray<FString> StrValueArray;

	const FString JsonFileName = GetConfigFileName(InCommandletName);
	TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(JsonFileName);
	if (JsonObject.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>> ArrayObjects = JsonObject->GetArrayField(JsonFieldName_Values);
		for (auto Value : ArrayObjects)
		{
			StrValueArray.Add(Value->AsString());
		}
	}

	return StrValueArray;
}

const FString FCommandletLauncherConfig::GetConfigFileName(const FString& InCommandletName)
{
	const FString JsonFileName = FPaths::GetBaseFilename(InCommandletName) + TEXT(".json");
	const FString UserSettingsPath = FPaths::Combine(FPaths::ProjectUserDir(), TEXT("Saved"), TEXT("CommandletLauncher"), JsonFileName);

	return UserSettingsPath;
}