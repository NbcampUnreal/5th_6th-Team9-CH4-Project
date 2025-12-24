#include "State/MoveState.h"

#include "GameMode/MainGameMode.h"
#include "Player/PlayerCharacter.h"
#include "State/PlayerStateMachine.h"

void UMoveState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveState OnEnter"));
	AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>();
	if (IsValid(GM) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode not found"));
		return;
	}

	AController* MPC = GetCameraPawn()->GetController();
	if (IsValid(MPC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller not found"));
		return;
	}

	const int32 DiceNumber = GM->ThrowDice(MPC);
	UE_LOG(LogTemp, Warning, TEXT("ThrowDice : %d"), DiceNumber);
	GetPlayerCharacter()->MoveToNextNode(DiceNumber);
}

void UMoveState::OnUpdate(float DeltaTime)
{
	if (GetPlayerCharacter()->remainingMove <= 0)
	{
		StateMachine->ChangeState(EStates::Idle);
	}
}

void UMoveState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveState OnExit"));
}

bool UMoveState::CanTakeDamage()
{
	return false;
}
