#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ui/EGameUIState.h"
#include "Ui/MinimapCameraActor.h"
#include "MyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiceResultReceived, int32, PlayerNumber, int32, DiceNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFirstReady, TArray<int32>, PlayerNumbers, TArray<int32>, DiceNums);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTurnEndInfo, TArray<int32>, PlayerNumbers, TArray<int32>, Scores);

class UInputMappingContext;
class UInputAction;
//Cho_sungmin
class UUserWidget;
UCLASS()
class TEAM9_PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

    void Server_RequestThrowDice_Implementation();

	//void IMCChange(Mode mode);

	// 서버 주사위 요청 RPC
    UFUNCTION(Server, Reliable)
    void Server_RequestThrowDice();

	// UI 상태 변경 Client RPC
    UFUNCTION(Client, Reliable)
    void Client_SetUIState(EGameUIState NewState);
	
	// 주사위 결과 수신 Client RPC
    UFUNCTION(Client, Reliable)
    void Client_ReceiveDiceResult(int32 PlayerNumber, int32 DiceNum);

	// 처음 정해진 순서 수신용 RPC
	UFUNCTION(Client, Reliable)
	void Client_ReceiveFirstOrder(TArray<int32> PlayerNumbers, TArray<int32> DiceNums);
	
	// 턴 종료시마다 순위 확인용 RPC
	UFUNCTION(Client, Reliable)
	void Client_ReceiveTurnEndInfo(TArray<int32> PlayerNumbers, TArray<int32> Scores);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ShowResult();

protected:
    void TestShowResult();

    //TEST
    virtual void SetupInputComponent() override;

public:

    UPROPERTY()
    AMinimapCameraActor* MinimapCamera;

	//누군가가 주사위를 굴릴때 실행
    UPROPERTY(BlueprintAssignable, Category = "Dice")
    FOnDiceResultReceived OnDiceResultReceived;

	//제일 처음 순서를 정할 때 실행
	UPROPERTY(BlueprintAssignable, Category = "Dice")
	FOnFirstReady OnFirstReady;

	//턴 종료시마다 플레이어 순위 확인용으로 실행
	UPROPERTY(BlueprintAssignable, Category = "Turn")
	FTurnEndInfo TurnEndInfo;

	UPROPERTY()
	UInputMappingContext* CurrentIMC;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* BoardIMC;

	UPROPERTY(EditAnywhere)
	UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere)
	UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere)
	UInputAction* CameraReturnAction;

	UPROPERTY(EditAnywhere)
	UInputAction* CameraKeyMoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* CameraWheelAction;
	
	// Cho_Sungmin - 인벤토리 위젯
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;
};

//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/PlayerController.h"
//#include "Ui/EGameUIState.h"
//#include "Ui/MinimapCameraActor.h"
//#include "MyTestPlayerController.generated.h"
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiceResultReceived, int32, Result);
//
//UCLASS()
//class TEAM9_PROJECT_API AMyTestPlayerController : public APlayerController
//{
//    GENERATED_BODY()
//
//public:
//
//    virtual void BeginPlay() override;
//
//    UPROPERTY()
//    AMinimapCameraActor* MinimapCamera;
//
//    // ���� �ֻ��� ��û RPC
//    UFUNCTION(Server, Reliable)
//    void Server_RequestThrowDice();
//
//    // UI ���� ���� Client RPC
//    UFUNCTION(Client, Reliable)
//    void Client_SetUIState(EGameUIState NewState);
//
//    // �ֻ��� ��� ���� Client RPC
//    UFUNCTION(Client, Reliable)
//    void Client_ReceiveDiceResult(int32 Result);
//
//    UPROPERTY(BlueprintAssignable, Category = "Dice")
//    FOnDiceResultReceived OnDiceResultReceived;
//
//    UFUNCTION(NetMulticast, Reliable)
//    void Multicast_ShowResult();
//
//    void Multicast_ShowResult_Implementation();
//
//protected:
//    void TestShowResult();
//
//    //TEST
//    virtual void SetupInputComponent() override;
//};