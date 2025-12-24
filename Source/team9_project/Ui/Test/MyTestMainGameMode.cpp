#include "MyTestMainGameMode.h"
#include "MyTestPlayerController.h"
#include "Ui/EGameUIState.h"

void AMyTestMainGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (AMyTestPlayerController* PC = Cast<AMyTestPlayerController>(NewPlayer))
    {
        // 5.5.4 버전에서는 클라이언트가 준비될 때까지 아주 짧은 지연을 주는 것이 좋습니다.
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, [PC]()
            {
                if (PC) PC->Client_SetUIState(EGameUIState::MainMenu);
            }, 0.1f, false);
    }
}