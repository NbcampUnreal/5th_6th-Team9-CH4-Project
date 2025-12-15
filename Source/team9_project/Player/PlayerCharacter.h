// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class AMyPlayerState;
class UPlayerStateMachine;
class ATestNode;
struct FTimerHandle;
struct FInputActionValue;

UCLASS()
class TEAM9_PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewControlle) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;


	int RandDice();

	void MoveToNextNode(int DiceValue);

private:
	void SetCurrentNode(ATestNode* TileNode);

	void UpdateMove();
	
	void OnDie();

	void LeftClickHandle(const FInputActionValue&);

	void RightClickHandle(const FInputActionValue&);


	UFUNCTION(Server, Reliable)
	void ServerRPCLeftClick();

	UFUNCTION(Server, Reliable)
	void ServerRPCRightClick();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSetRotation(FRotator NewRotation);

public:
	UPROPERTY()
	AMyPlayerState* MyPlayerState;

	UPROPERTY()
	UPlayerStateMachine* StateMachine;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;


	UPROPERTY(EditAnywhere)
	ATestNode* CurrentNode;

	
private:
	FTimerHandle MoveTimerHandle;

	FVector MoveStart;

	FVector MoveTarget;

	float MoveSpeed;

	float MoveDuration;

	float MoveElapsed;

	int remainingMove;
};
