#include "State/StateBase.h"

#include "State/PlayerStateMachine.h"

void UStateBase::Initialize(UPlayerStateMachine* NewStateMachine)
{
	StateMachine = NewStateMachine;
}

void UStateBase::OnEnter()
{

}

void UStateBase::OnUpdate(float DeltaTime)
{

}

void UStateBase::OnExit()
{

}

void UStateBase::Move()
{

}

bool UStateBase::CanTakeDamage()
{
	return false;
}


UPlayerStateMachine* UStateBase::GetStateMachine() const
{
	return StateMachine;
}

APlayerCharacter* UStateBase::GetPlayerCharacter() const
{
	return StateMachine->GetOwnerCharacter();;
}