#include "MyTestGameMode.h"
#include "MyTestPlayerController.h"
#include "Ui/EGameUIState.h"

void AMyTestGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // 인게임 접속 시 HUD 위젯으로 교체 명령
    if (AMyTestPlayerController* PC = Cast<AMyTestPlayerController>(NewPlayer))
    {
        PC->Client_SetUIState(EGameUIState::InGame);
    }
}

void AMyTestGameMode::ProcessDiceThrow(AMyTestPlayerController* Requester)
{
    if (!Requester) return;

    // 서버 난수 생성
    int32 DiceResult = FMath::RandRange(1, 6);

    // 클라이언트에게 결과 전송
    Requester->Client_ReceiveDiceResult(DiceResult);
}