#include "State/ItemUseState.h"

#include "State/PlayerStateMachine.h"
#include "Player/CameraPawn.h"
#include "Player/PlayerCharacter.h"

void UItemUseState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemUseState OnEnter"));

	GetPlayerCharacter()->bIsUsingItem = true;
}

void UItemUseState::OnUpdate(float DeltaTime)
{
	ACameraPawn* CP = Cast<ACameraPawn>(GetCameraPawn());
	if (CP ->GetIsUsingItem())
	{
		GetStateMachine()->ChangeState(EStates::Idle);
	}
}

void UItemUseState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemUseState OnExit"));

	GetPlayerCharacter()->bIsUsingItem = false;
}

void UItemUseState::Move()
{
}

void UItemUseState::Hit()
{
}

bool UItemUseState::CanTakeDamage()
{
	return false;
}
