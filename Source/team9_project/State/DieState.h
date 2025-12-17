#pragma once

#include "CoreMinimal.h"
#include "State/StateBase.h"
#include "DieState.generated.h"

UCLASS()
class TEAM9_PROJECT_API UDieState : public UStateBase
{
	GENERATED_BODY()
	
	virtual void OnEnter() override;

	virtual void OnUpdate(float DeltaTime) override;

	virtual void OnExit() override;

	virtual bool CanTakeDamage() override;
};
