#include "State/MoveState.h"

#include "Player/PlayerCharacter.h"
#include "State/PlayerStateMachine.h"

void UMoveState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveState OnEnter"));
}

void UMoveState::OnUpdate(float DeltaTime)
{
}

void UMoveState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveState OnExit"));
}

bool UMoveState::CanTakeDamage()
{
	return false;
}
