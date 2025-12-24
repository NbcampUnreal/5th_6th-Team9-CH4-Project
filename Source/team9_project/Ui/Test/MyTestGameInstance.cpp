#include "MyTestGameInstance.h"
#include "Ui/UIManagerSubsystem.h"

UMyTestGameInstance::UMyTestGameInstance()
{
    CurrentRound = 1;
}

void UMyTestGameInstance::Init()
{
    Super::Init();

    // 1. UI 매니저 서브시스템을 가져옴
    if (UUIManagerSubsystem* UISubsystem = GetSubsystem<UUIManagerSubsystem>())
    {
        // 2. 에디터에서 설정한 TMap을 순회함
        for (const auto& Pair : TestUIWidgetMap)
        {
            // 3. 서브시스템에 각 상태별 위젯 클래스를 등록(Register)함
            UISubsystem->RegisterUIWidget(Pair.Key, Pair.Value);
        }
        UE_LOG(LogTemp, Log, TEXT("[Success] All UI Widgets Registered!"));
    }
}