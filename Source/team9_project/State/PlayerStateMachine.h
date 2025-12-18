#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerStateMachine.generated.h"

class APlayerCharacter;
class UStateBase;
class UIdleState;
class UMoveState;
class UDieState;
class UHitState;
class UItemUseState;

UENUM(BlueprintType)
enum class EStates : uint8
{
	Idle,
	Moving,
	Die,
	Hit,
	ItemUse
};

UCLASS()
class TEAM9_PROJECT_API UPlayerStateMachine : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(APawn* Player, APlayerCharacter* Character);

	virtual void OnUpdate(float DeltaTime);

	void ChangeState(EStates NewState);

	UStateBase* GetCurrentState() const;

	APlayerCharacter* GetPlayerCharacter() const;

	APawn* GetCameraPawn() const;

protected:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<APawn> CameraPawn;

	UPROPERTY()
	TObjectPtr<UStateBase> CurrentState;

	UPROPERTY()
	TObjectPtr<UIdleState> IdleState;

	UPROPERTY()
	TObjectPtr <UMoveState> MoveState;

	UPROPERTY()
	TObjectPtr <UDieState> DieState;

	UPROPERTY()
	TObjectPtr <UHitState> HitState;

};
