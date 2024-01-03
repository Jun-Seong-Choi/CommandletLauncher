#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformProcess.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "CommandletLauncher.h"

namespace ECommandletProcessStatus
{
    enum Type
    {
        Pending,
        Processing,
        Canceled,
        Completed,
        Terminated,
    };
}

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandletProcessStartedDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandletProcessCanceledDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandletProcessCompletedDelegate, const FString&);

class FCommandletLauncherProcess : public FRunnable
{
public:

    FCommandletLauncherProcess(const FString& InName, const FString InArgs, void* InReadPipe, void* InWritePipe, FProcHandle& InProcessHandle, class FCommandletLauncher* InLauncher) :
        Name(InName),
        Args(InArgs),
        ReadPipe(InReadPipe),
        WritePipe(InWritePipe),
        ProcessHandle(InProcessHandle)
    {
        //InLauncher->OnGetMessageReceived().AddRaw(this, &FCommandletLauncherProcess::HandleMessageReceived);
    }

    ~FCommandletLauncherProcess()
    {
        if (Thread != nullptr)
        {
            Thread->Kill(true);
            delete Thread;
            Thread = nullptr;
        }

        FPlatformProcess::CloseProc(ProcessHandle);
    }

public:

    //~ Begin FRunnable Interface

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    //~ End FRunnable Interface

public:

    bool IsValid();

    void Execute();
    void Cancel();
    void Finish();

    const FString GetName();
    const FString GetArguments();

    virtual FOnCommandletProcessStartedDelegate& OnStarted()
    {
        return CommandletStarted;
    }

    virtual FOnCommandletProcessCanceledDelegate& OnCanceled()
    {
        return CommandletCanceled;
    }

    virtual FOnCommandletProcessCompletedDelegate& OnCompleted()
    {
        return CommandletCompleted;
    }

    virtual uint32 GetWarningCount() const
    {
        return WarningCounter;
    }

    virtual uint32 GetErrorCount() const
    {
        return ErrorCounter;
    }

private:

    bool IsAlive();

    void Processing();

    void HandleMessageReceived(const FString& InMessage);

private:

    FString Name;
    FString Args;

    uint32 ErrorCounter = 0;
    uint32 WarningCounter = 0;

    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;

    bool bCancelled = false;

    FProcHandle& ProcessHandle;

    ECommandletProcessStatus::Type Status = ECommandletProcessStatus::Pending;

    FOnCommandletProcessStartedDelegate CommandletStarted;
    FOnCommandletProcessCanceledDelegate CommandletCanceled;
    FOnCommandletProcessCompletedDelegate CommandletCompleted;

    FRunnableThread* Thread = nullptr;
};