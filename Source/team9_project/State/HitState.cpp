#include "State/HitState.h"

#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"
#include "State/PlayerStateMachine.h"

void UHitState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("HitState OnEnter"));
	ElapsedTime = 0.f;
	HitDuration = 2.f;
}

void UHitState::OnUpdate(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	if (ElapsedTime >= HitDuration)
	{
		GetStateMachine()->ChangeState(EStates::Idle);
	}
	
	if (GetPlayerCharacter()->OnDie())
	{
		GetStateMachine()->ChangeState(EStates::Die);
	}
}

void UHitState::OnExit()
{
	UE_LOG(LogTemp, Warning, TEXT("HitState OnEnter"));
}

bool UHitState::CanTakeDamage()
{
	return true;
}
