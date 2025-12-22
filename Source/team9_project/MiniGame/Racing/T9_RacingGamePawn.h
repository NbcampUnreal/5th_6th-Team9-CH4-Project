#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGamePawnBase.h"
#include "InputActionValue.h"
#include "T9_RacingGamePawn.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TEAM9_PROJECT_API AT9_RacingGamePawn : public AT9_MiniGamePawnBase
{
	GENERATED_BODY()
	
public:
	AT9_RacingGamePawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Input_Forward(const FInputActionValue& Value);

	void Input_Turn(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_Forward();

	UFUNCTION(Server, Reliable)
	void Server_Turn(float Value);

	void SimulateMove(float DeltaTime);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Racing;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Forward;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AccelPerPress = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxSpeed = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Decel = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TurnSpeed = 120.f;

	float CurrentSpeed = 0.f;

	float TurnInput = 0.f;
};
