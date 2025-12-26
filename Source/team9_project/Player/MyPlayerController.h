#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ui/EGameUIState.h"
#include "Ui/MinimapCameraActor.h"
#include "MyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiceResultReceived, int32, Result);

class UInputMappingContext;
class UInputAction;

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
    void Client_ReceiveDiceResult(int32 Result);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ShowResult();

    void Multicast_ShowResult_Implementation();

protected:
    void TestShowResult();

    //TEST
    virtual void SetupInputComponent() override;

public:

    UPROPERTY()
    AMinimapCameraActor* MinimapCamera;

    UPROPERTY(BlueprintAssignable, Category = "Dice")
    FOnDiceResultReceived OnDiceResultReceived;

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
};