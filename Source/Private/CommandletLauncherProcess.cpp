#include "CommandletLauncherProcess.h"
#include "UnrealEdMisc.h"

DEFINE_LOG_CATEGORY_STATIC(CommandletLauncherProcess, Log, All);

bool FCommandletLauncherProcess::Init()
{
    return true;
}

uint32 FCommandletLauncherProcess::Run()
{
    FPlatformProcess::Sleep(0.1f);

    Processing();

    return 0;
}

void FCommandletLauncherProcess::Stop()
{
}

void FCommandletLauncherProcess::Exit()
{
}

bool FCommandletLauncherProcess::IsValid()
{
    UClass* CommandletClass = nullptr;

    if (Name.Len())
    {
        CommandletClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *Name, EFindFirstObjectOptions::None, ELogVerbosity::Warning, TEXT("looking for commandlet")));

        int32 PeriodIdx;
        if (!CommandletClass && Name.FindChar('.', PeriodIdx))
        {
            FModuleManager::Get().LoadModule(*Name.Left(PeriodIdx));
            CommandletClass = FindFirstObject<UClass>(*Name, EFindFirstObjectOptions::None, ELogVerbosity::Warning, TEXT("Looking for commandlet class"));
        }

        if (!CommandletClass)
        {
            UE_LOG(CommandletLauncherProcess, Error, TEXT("%s looked like a commandlet, but we could not find the class."), *Name);
        }
    }

    return CommandletClass != nullptr;
}

void FCommandletLauncherProcess::Execute()
{
    check(Status == ECommandletProcessStatus::Pending);
    Thread = FRunnableThread::Create(this, *Name);
}

void FCommandletLauncherProcess::Cancel()
{
    check(Status == ECommandletProcessStatus::Processing);
    bCancelled = true;
}

void FCommandletLauncherProcess::Finish()
{
    FPlatformProcess::TerminateProc(ProcessHandle);
}

bool FCommandletLauncherProcess::IsAlive()
{
    return Status != ECommandletProcessStatus::Terminated;
}

void FCommandletLauncherProcess::Processing()
{
    while (IsAlive())
    {
        switch (Status)
        {
        case ECommandletProcessStatus::Pending:
        {
            FString ExecutableName = FString(FPlatformProcess::ExecutablePath());
            {
                if (ExecutableName.EndsWith(".exe", ESearchCase::IgnoreCase) && !FPaths::GetBaseFilename(ExecutableName).EndsWith("-cmd", ESearchCase::IgnoreCase))
                {
                    FString NewExeName = ExecutableName.Left(ExecutableName.Len() - 4) + "-Cmd.exe";
                    if (FPaths::FileExists(NewExeName))
                    {
                        ExecutableName = NewExeName;
                    }
                }
            }

            const FString ProjectPath = FPaths::IsProjectFilePathSet() ? FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()) : FPaths::ConvertRelativePathToFull(FApp::GetProjectName());
            const FString Arguments = FString::Printf(TEXT("\"%s\" -run=%s %s"), *ProjectPath, *Name, *Args);

            uint32 OutProcessId = 0;
            ProcessHandle = FPlatformProcess::CreateProc(*ExecutableName, *Arguments, false, true, true, &OutProcessId, 0, nullptr, WritePipe, ReadPipe);
            if (ProcessHandle.IsValid())
            {
                Status = ECommandletProcessStatus::Processing;
            }

            CommandletStarted.Broadcast(Name);
        }
        break;
        case ECommandletProcessStatus::Processing:
        {
            int32 Result = 0;
            if (!bCancelled && FPlatformProcess::GetProcReturnCode(ProcessHandle, &Result))
            {
                Status = ECommandletProcessStatus::Completed;
            }
            else if (bCancelled)
            {
                Status = ECommandletProcessStatus::Canceled;
            }
        }
        break;
        case ECommandletProcessStatus::Canceled:
        {
            CommandletCanceled.Broadcast(Name);
            Status = ECommandletProcessStatus::Terminated;
        }
        break;
        case ECommandletProcessStatus::Completed:
        {
            CommandletCompleted.Broadcast(Name);
            Status = ECommandletProcessStatus::Terminated;
        }
        break;
        default:
        {
        }
        }
    }
}

void FCommandletLauncherProcess::HandleMessageReceived(const FString& InMessage)
{
    if (InMessage.Contains(TEXT("Error:"), ESearchCase::IgnoreCase))
    {
        ++ErrorCounter;
    }
    else if (InMessage.Contains(TEXT("Warning:"), ESearchCase::IgnoreCase))
    {
        ++WarningCounter;
    }
}

const FString FCommandletLauncherProcess::GetName()
{
    return Name;
}

const FString FCommandletLauncherProcess::GetArguments()
{
    const FString CommandletArguments = FString::Format(TEXT("\"{0}\""), { FPaths::SetExtension(FPaths::Combine(FPaths::ProjectDir(), FApp::GetProjectName()), ".uproject") }) + " -run=" + Name + " " + Args;
    return CommandletArguments;
}