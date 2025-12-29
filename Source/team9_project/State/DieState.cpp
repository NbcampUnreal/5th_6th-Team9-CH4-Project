#include "State/DieState.h"

#include "Player/PlayerCharacter.h"

void UDieState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("DieState OnEnter"));
	GetPlayerCharacter()->bIsDie = true;
	UE_LOG(LogTemp, Warning, TEXT("bIsDie : true"));
}

void UDieState::OnUpdate(float DeltaTime)
{
}

void UDieState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("DieState OnExit"));
	GetPlayerCharacter()->bIsDie = false;
	UE_LOG(LogTemp, Warning, TEXT("bIsDie : false"));
}

bool UDieState::CanTakeDamage()
{
	return false;
}
