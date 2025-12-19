#pragma once

#include "CoreMinimal.h"
#include "State/StateBase.h"
#include "HitState.generated.h"

UCLASS()
class TEAM9_PROJECT_API UHitState : public UStateBase
{
	GENERATED_BODY()
	
	virtual void OnEnter() override;

	virtual void OnUpdate(float DeltaTime) override;

	virtual void OnExit() override;

	virtual bool CanTakeDamage() override;

private:
	float ElapsedTime;
	float HitDuration; // 1~2초
};
