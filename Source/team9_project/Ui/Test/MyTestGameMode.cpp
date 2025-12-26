#include "MyTestGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Ui/EGameUIState.h"
#include "Player/MyPlayerState.h"
#include "Player/MyPlayerController.h"

void AMyTestGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // �ΰ��� ���� �� HUD �������� ��ü ����
    if (AMyPlayerController* PC = Cast<AMyPlayerController>(NewPlayer))
    {
        PC->Client_SetUIState(EGameUIState::InGame);
    }
}

void AMyTestGameMode::ProcessDiceThrow(AMyPlayerController* Requester)
{
    if (!Requester) return;

    // ���� ���� ����
    int32 DiceResult = FMath::RandRange(1, 6);

    // Ŭ���̾�Ʈ���� ��� ����
    Requester->Client_ReceiveDiceResult(1, DiceResult);
}

void AMyTestGameMode::EndMatch()
{
    // 1. �������� ��� �÷��̾��� ������ ������� ���� ���
    CalculateFinalResults();

    // 2. ��� �÷��̾� ��Ʈ�ѷ��� ��ȸ�ϸ� ���â ǥ�� RPC ȣ��
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AMyPlayerController* PC = Cast<AMyPlayerController>(It->Get()))
        {
            // ��� Ŭ���̾�Ʈ�� ���â�� ��쵵�� ��Ƽĳ��Ʈ ȣ��
            PC->Multicast_ShowResult();
        }
    }
}

void AMyTestGameMode::CalculateFinalResults()
{
    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS) return;

    TArray<AMyPlayerState*> PlayerStates;
    for (APlayerState* PS : GS->PlayerArray)
    {
        if (AMyPlayerState* TestPS = Cast<AMyPlayerState>(PS))
        {
            PlayerStates.Add(TestPS);
        }
    }

    if (PlayerStates.Num() == 0) return;

    // ���� ���� ������ ���� (��������)
    PlayerStates.Sort([](const AMyPlayerState& A, const AMyPlayerState& B)
        {
            return A.CurrentScore > B.CurrentScore;  // ���� ������ 1��
        });

    // ���� �ο�
    for (int32 i = 0; i < PlayerStates.Num(); ++i)
    {
        PlayerStates[i]->FinalRank = i + 1;

        //UE_LOG(LogTemp, Warning, TEXT("[GameMode] %d: %s (Scor: %d)"), PlayerStates[i]->FinalRank, *PlayerStates[i]->DisplayName.IsEmpty() ? PlayerStates[i]->GetPlayerName() : PlayerStates[i]->DisplayName, PlayerStates[i]->CurrentScore);
    }
}