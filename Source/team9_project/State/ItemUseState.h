#pragma once

#include "CoreMinimal.h"
#include "State/StateBase.h"
#include "ItemUseState.generated.h"

UCLASS()
class TEAM9_PROJECT_API UItemUseState : public UStateBase
{
	GENERATED_BODY()
	
	virtual void OnEnter() override;

	virtual void OnUpdate(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void Move() override;

	virtual void Hit() override;

	virtual bool CanTakeDamage() override;

};
