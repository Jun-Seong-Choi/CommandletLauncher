#include "CommandletLauncher.h"
#include "CommandletLauncherProcess.h"

DEFINE_LOG_CATEGORY_STATIC(CommandletLauncher, Log, All);

bool FCommandletLauncher::Init()
{
    return true;
}

uint32 FCommandletLauncher::Run()
{
    FPlatformProcess::Sleep(0.1f);

    Processing();

    return 0;
}

void FCommandletLauncher::Stop()
{
}

void FCommandletLauncher::Exit()
{
}

void FCommandletLauncher::CreateNewCommandletProcess(const FString& InName, const FString& InArgs)
{
    TSharedPtr<class FCommandletLauncherProcess> NewCommandlet = MakeShared<FCommandletLauncherProcess>(InName, InArgs, ReadPipe, WritePipe, ProcessHandle, this);
    if (NewCommandlet->IsValid())
    {
        NewCommandlet->OnStarted().AddRaw(this, &FCommandletLauncher::OnCommandletStarted);
        NewCommandlet->OnCanceled().AddRaw(this, &FCommandletLauncher::OnCommandletCanceled);
        NewCommandlet->OnCompleted().AddRaw(this, &FCommandletLauncher::OnCommandletCompleted);
        if (NewCommandlet.IsValid())
        {
            Commandlets.Enqueue(NewCommandlet);
        }
    }
}

void FCommandletLauncher::OnCommandletStarted(const FString& InName)
{
    CommandletStartTime = FPlatformTime::Seconds();
    OnStarted.Broadcast(InName);
}

void FCommandletLauncher::OnCommandletCanceled(const FString& InName)
{
    Status = ECommandletLauncherStatus::Completed;
    OnCanceled.Broadcast(InName);
}

void FCommandletLauncher::OnCommandletCompleted(const FString& InName)
{
    Status = ECommandletLauncherStatus::Completed;
    OnCompleted.Broadcast(InName, FPlatformTime::Seconds() - CommandletStartTime);
}

void FCommandletLauncher::OnStop()
{
    bForceStop = true;
}

void FCommandletLauncher::Processing()
{
    while (!bForceStop)
    {
        switch (Status)
        {
        case ECommandletLauncherStatus::Waiting:
        {
            if (!Commandlets.IsEmpty())
            {
                Commandlets.Dequeue(ProcessingCommandlet);

                if (ProcessingCommandlet.IsValid())
                {
                    ProcessingCommandlet->Execute();
                }

                Status = ECommandletLauncherStatus::Processing;
            }
        }
        break;
        case ECommandletLauncherStatus::Processing:
        {
            if (FPlatformProcess::IsProcRunning(ProcessHandle))
            {
                FPlatformProcess::Sleep(0.1f);

                FString NewLine = FPlatformProcess::ReadPipe(ReadPipe);
                if (NewLine.Len() > 0)
                {
                    TArray<FString> StringArray;
                    int32 Count = NewLine.ParseIntoArray(StringArray, TEXT("\n"), true);
                    if (Count > 1)
                    {
                        for (int32 Index = 0; Index < Count - 1; ++Index)
                        {
                            StringArray[Index].TrimEndInline();
                            {
                                OutputMessage(StringArray[Index]);
                            }
                        }
                    }
                }
            }
        }
        break;
        case ECommandletLauncherStatus::Canceled:
        {
            ProcessingCommandlet->Cancel();
        }
        break;
        case ECommandletLauncherStatus::Completed:
        {
            ProcessingCommandlet->Finish();
            ProcessingCommandlet = nullptr;

            Status = ECommandletLauncherStatus::Waiting;
        }
        break;
        }
    }
}

void FCommandletLauncher::OutputMessage(const FString& InMessage)
{
    TArray<FString> StringArray;
    if (InMessage.Contains(ProcessingCommandlet->GetName()))
    {
        const FString& Delim = FString::Format(TEXT("{0}:"), { ProcessingCommandlet->GetName() });
        InMessage.ParseIntoArray(StringArray, *Delim, true);

        const FString& ResultMessageString = TEXT("LogInit: Display:");
        if (!StringArray.IsEmpty() && !StringArray[0].Contains(ResultMessageString))
        {
            const FString& OutMessage = FString::Format(TEXT("{0}{1}"), { Delim, StringArray[1] });
            {
                UE_LOG(CommandletLauncher, Log, TEXT("%s"), *OutMessage);
                OnMessageReceived.Broadcast(OutMessage);
            }
        }
    }
}