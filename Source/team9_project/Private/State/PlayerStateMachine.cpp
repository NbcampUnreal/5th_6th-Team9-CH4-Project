#include "State/PlayerStateMachine.h"

#include "Player/PlayerCharacter.h"
#include "State/StateBase.h"
#include "State/PlayerStateMachine.h"
#include "State/IdleState.h"
#include "State/MoveState.h"
#include "State/DieState.h"
#include "State/HitState.h"

void UPlayerStateMachine::Initialize(APlayerCharacter* InOwner)
{
	OwnerCharacter = InOwner;
	CurrentState = nullptr;

	IdleState = NewObject<UIdleState>(this);
	if (IdleState)
	{
		IdleState->Initialize(this);
	}

	MoveState = NewObject<UMoveState>(this);
	if (MoveState)
	{
		MoveState->Initialize(this);
	}

	DieState = NewObject<UDieState>(this);
	if (DieState)
	{
		DieState->Initialize(this);
	}

	HitState = NewObject<UHitState>(this);
	if (HitState)
	{
		HitState->Initialize(this);
	}

	ChangeState(EStates::Idle);
}

void UPlayerStateMachine::OnUpdate(float DeltaTime)
{
	if (CurrentState)
	{
		CurrentState->OnUpdate(DeltaTime);
	}
}

void UPlayerStateMachine::ChangeState(EStates NewState)
{
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	switch (NewState)
	{
	case EStates::Idle:
		CurrentState = IdleState;
		break;
	case EStates::Moving:
		CurrentState = MoveState;
		break;
	case EStates::Die:
		CurrentState = DieState;
		break;
	case EStates::Hit:
		CurrentState = HitState;
		break;

	default:
		CurrentState = IdleState;
		break;
	}

	if (CurrentState)
	{
		CurrentState->OnEnter();
	}
}

UStateBase* UPlayerStateMachine::GetCurrentState() const
{
	return CurrentState;
}

APlayerCharacter* UPlayerStateMachine::GetOwnerCharacter() const
{
	return OwnerCharacter;
}
