#include "MyTestGameMode.h"
#include "MyTestPlayerController.h"
#include "Ui/Test/MyTestPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
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

void AMyTestGameMode::EndMatch()
{
    // 1. 서버에서 모든 플레이어의 점수를 기반으로 순위 계산
    CalculateFinalResults();

    // 2. 모든 플레이어 컨트롤러를 순회하며 결과창 표시 RPC 호출
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AMyTestPlayerController* PC = Cast<AMyTestPlayerController>(It->Get()))
        {
            // 모든 클라이언트가 결과창을 띄우도록 멀티캐스트 호출
            PC->Multicast_ShowResult();
        }
    }
}

void AMyTestGameMode::CalculateFinalResults()
{
    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS) return;

    TArray<AMyTestPlayerState*> PlayerStates;
    for (APlayerState* PS : GS->PlayerArray)
    {
        if (AMyTestPlayerState* TestPS = Cast<AMyTestPlayerState>(PS))
        {
            PlayerStates.Add(TestPS);
        }
    }

    if (PlayerStates.Num() == 0) return;

    // 점수 높은 순으로 정렬 (내림차순)
    PlayerStates.Sort([](const AMyTestPlayerState& A, const AMyTestPlayerState& B)
        {
            return A.FinalScore > B.FinalScore;  // 높은 점수가 1위
        });

    // 순위 부여
    for (int32 i = 0; i < PlayerStates.Num(); ++i)
    {
        PlayerStates[i]->FinalRank = i + 1;

        //UE_LOG(LogTemp, Warning, TEXT("[GameMode] %d: %s (Scor: %d)"), PlayerStates[i]->FinalRank, *PlayerStates[i]->DisplayName.IsEmpty() ? PlayerStates[i]->GetPlayerName() : PlayerStates[i]->DisplayName, PlayerStates[i]->FinalScore);
    }
}