#include "MyTestGameInstance.h"
#include "Ui/UIManagerSubsystem.h"

UMyTestGameInstance::UMyTestGameInstance()
{
    CurrentRound = 1;
}

void UMyTestGameInstance::Init()
{
    Super::Init();
    if (UUIManagerSubsystem* UISubsystem = GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->Test();
    }
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMyTestGameInstance::Callback_PostLoadMapWithWorld);

}

void UMyTestGameInstance::Callback_PostLoadMapWithWorld(UWorld* inWorld)
{
    if (UUIManagerSubsystem* UISubsystem = GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->Test();
    }
}
