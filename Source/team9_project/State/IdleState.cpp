#include "State/IdleState.h"

#include "Player/PlayerCharacter.h"
#include "State/PlayerStateMachine.h"

void UIdleState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("IdleState OnEnter"));
}

void UIdleState::OnUpdate(float DeltaTime)
{
}

void UIdleState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("IdleState OnExit"));
}

void UIdleState::Move()
{
	StateMachine->ChangeState(EStates::Moving);
}

void UIdleState::Hit()
{
	StateMachine->ChangeState(EStates::Hit);
}

bool UIdleState::CanTakeDamage()
{
	return true;
}
