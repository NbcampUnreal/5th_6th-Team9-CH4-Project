#include "State/DieState.h"

void UDieState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("DieState OnEnter"));
}

void UDieState::OnUpdate(float DeltaTime)
{
}

void UDieState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("DieState OnExit"));
}

bool UDieState::CanTakeDamage()
{
	return false;
}
