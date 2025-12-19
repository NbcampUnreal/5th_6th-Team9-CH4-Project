#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

class UPlayerStateMachine;
class APlayerCharacter;

UCLASS()
class TEAM9_PROJECT_API UStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(UPlayerStateMachine* NewStateMachine);

	virtual void OnEnter();

	virtual void OnUpdate(float DeltaTime);

	virtual void OnExit();

	virtual void Move();

	virtual void Hit();

	virtual bool CanTakeDamage();


	UPlayerStateMachine* GetStateMachine() const;

	APlayerCharacter* GetPlayerCharacter() const;

	APawn* GetCameraPawn() const;


protected:
	UPROPERTY()
	TObjectPtr<UPlayerStateMachine> StateMachine;
};
