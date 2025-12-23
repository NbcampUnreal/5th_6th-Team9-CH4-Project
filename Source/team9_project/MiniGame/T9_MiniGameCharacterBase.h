#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "T9_MiniGameCharacterBase.generated.h"

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGameCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AT9_MiniGameCharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EnableCharacterInput();

	virtual void DisableCharacterInput();

protected:
	bool bInputEnabled;

};
