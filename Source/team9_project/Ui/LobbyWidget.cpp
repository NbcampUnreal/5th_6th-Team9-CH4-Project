// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/LobbyWidget.h"
#include "State/StateBase.h"
//#include "State/PlayerStateMachine.h"
#include "GameFramework/GameStateBase.h" // 추가: AGameStateBase 정의 포함

void ULobbyWidget::UpdatePlayerList()
{
    if (!PlayerListContainer || !LobbySlotClass) return;

    PlayerListContainer->ClearChildren();

    // GameState를 통해 현재 접속한 플레이어 배열(PlayerArray)을 가져옴
    AGameStateBase* GS = GetWorld()->GetGameState();
    if (GS)
    {
        for (APlayerState* PS : GS->PlayerArray)
        {
            UUserWidget* PlayerSlotWidget = CreateWidget<UUserWidget>(this, LobbySlotClass);
            PlayerListContainer->AddChild(PlayerSlotWidget);
            // [TODO] Slot 위젯에 PS->GetPlayerName() 등을 전달하여 UI 업데이트
        }
    }

    // 방장 여부에 따라 버튼 텍스트/기능 변경
    if (GetOwningPlayer()->HasAuthority())
    {
        // Host: 라운드 설정 가능, 시작 버튼 활성화
        RoundSettingSpinner->SetIsEnabled(true);
        // Btn_Action 텍스트 -> "Start Game"
    }
    else
    {
        // Client: 라운드 설정 불가, 준비 버튼 활성화
        RoundSettingSpinner->SetIsEnabled(false);
        // Btn_Action 텍스트 -> "Ready"
    }
}

void ULobbyWidget::OnActionClicked()
{
    if (GetOwningPlayer()->HasAuthority())
    {
        // Host: ServerTravel을 통해 게임 레벨로 이동
        GetWorld()->ServerTravel("/Game/Maps/GameMap?listen");
    }
    else
    {
        // Client: Server에 Ready 상태 전송 (RPC)
        // MyPlayerController->ServerSetReady(true);
    }
}