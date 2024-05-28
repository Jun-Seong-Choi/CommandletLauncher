#pragma once

#include "CoreMinimal.h"
#include "CommandletLauncherProcess.h"

namespace ECommandletLauncherStatus
{
    enum Type
    {
        Waiting,
        Processing,
        Canceled,
        Completed
    };
}

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandletMessageReceivedDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandletStartedDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandletCanceledDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommandletCompletedDelegate, const FString&, double);

class FCommandletLauncher : public FRunnable
{
public:
    FCommandletLauncher()
    {
        if (Thread == nullptr)
        {
            Thread = FRunnableThread::Create(this, TEXT("CommandletLauncher"));
        }

        FPlatformProcess::CreatePipe(ReadPipe, WritePipe);
    }

    ~FCommandletLauncher()
    {
        if (Thread != nullptr)
        {
            Thread->Kill(true);
            delete Thread;
            Thread = nullptr;
        }

        FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
    }

public:
    //~ Begin FRunnable Interface

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    //~ End FRunnable Interface

public:
    void CreateNewCommandletProcess(const FString& InName, const FString& InArgs);

    void OnCommandletStarted(const FString& InName);
    void OnCommandletCanceled(const FString& InName);
    void OnCommandletCompleted(const FString& InName);

    void OnStop();

    FOnCommandletMessageReceivedDelegate& OnGetMessageReceived()
    {
        return OnMessageReceived;
    }

    FOnCommandletStartedDelegate& OnGetStarted()
    {
        return OnStarted;
    }

    FOnCommandletCompletedDelegate& OnGetCompleted()
    {
        return OnCompleted;
    }

private:
    void Processing();
    void OutputMessage(const FString& InMessage);

private:
    ECommandletLauncherStatus::Type Status = ECommandletLauncherStatus::Waiting;

    TQueue<TSharedPtr<class FCommandletLauncherProcess>> Commandlets;

    TSharedPtr<class FCommandletLauncherProcess> ProcessingCommandlet;

    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;
    FProcHandle ProcessHandle;

    double LaunchStartTime = 0.0;
    double CommandletStartTime = 0.0;

    bool bForceStop = false;

    FOnCommandletMessageReceivedDelegate OnMessageReceived;
    FOnCommandletStartedDelegate OnStarted;
    FOnCommandletCanceledDelegate OnCanceled;
    FOnCommandletCompletedDelegate OnCompleted;

    FRunnableThread* Thread = nullptr;
};
