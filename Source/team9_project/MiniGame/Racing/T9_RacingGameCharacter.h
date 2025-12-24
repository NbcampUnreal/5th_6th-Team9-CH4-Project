#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGameCharacterBase.h"
#include "InputActionValue.h"
#include "T9_RacingGameCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TEAM9_PROJECT_API AT9_RacingGameCharacter : public AT9_MiniGameCharacterBase
{
	GENERATED_BODY()
	
public:
	AT9_RacingGameCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetAcceptInput(bool bEnable) override;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UCameraComponent> Camera;

private:
	void HandleMoveInput(const FInputActionValue& InValue);

	void HandleLookInput(const FInputActionValue& InValue);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Racing")
	float CurrentSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Racing")
	float MaxSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Racing")
	float AccelPerInput = 50.f; 

	UPROPERTY(EditAnywhere, Category = "Racing")
	float DecelPerSecond = 400.f;  

	bool bAccelRequested = false;
};
