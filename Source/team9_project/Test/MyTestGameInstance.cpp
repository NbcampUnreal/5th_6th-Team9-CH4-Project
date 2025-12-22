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
        for (const auto& Pair : TestUIWidgetMap)
        {
            UISubsystem->RegisterUIWidget(Pair.Key, Pair.Value);
        }
        UE_LOG(LogTemp, Log, TEXT("[Success] All UI Widgets Registered!"));
    }
}