#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ui/EGameUIState.h"
#include "MyTestPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiceResultReceived, int32, Result);

UCLASS()
class TEAM9_PROJECT_API AMyTestPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // 서버 주사위 요청 RPC
    UFUNCTION(Server, Reliable)
    void Server_RequestThrowDice();

    // UI 상태 변경 Client RPC
    UFUNCTION(Client, Reliable)
    void Client_SetUIState(EGameUIState NewState);

    // 주사위 결과 수신 Client RPC
    UFUNCTION(Client, Reliable)
    void Client_ReceiveDiceResult(int32 Result);

    UPROPERTY(BlueprintAssignable, Category = "Dice")
    FOnDiceResultReceived OnDiceResultReceived;
};