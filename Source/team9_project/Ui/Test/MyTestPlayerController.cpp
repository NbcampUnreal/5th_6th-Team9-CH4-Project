#include "MyTestPlayerController.h"
#include "MyTestGameMode.h"
#include "Ui/UIManagerSubsystem.h"

// 핵심: 모든 RPC는 뒤에 _Implementation을 붙여서 본체를 만듭니다.

void AMyTestPlayerController::Server_RequestThrowDice_Implementation()
{
    // AMyTestGameMode로 캐스팅하여 주사위 로직 처리 요청
    if (AMyTestGameMode* GM = GetWorld()->GetAuthGameMode<AMyTestGameMode>())
    {
        GM->ProcessDiceThrow(this);
    }
}

void AMyTestPlayerController::Client_SetUIState_Implementation(EGameUIState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("Client_SetUIState 실행 상태: %s"), *UEnum::GetValueAsString(NewState));
    // UUIManagerSubsystem을 사용하여 중앙 제어식 UI 변경
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->SetUIState(NewState);
    }
}

void AMyTestPlayerController::Client_ReceiveDiceResult_Implementation(int32 Result)
{
    // UI나 블루프린트에서 들을 수 있도록 델리게이트 브로드캐스트
    OnDiceResultReceived.Broadcast(Result);
}