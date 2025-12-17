// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponent;
struct FTimerHandle;

UCLASS()
class TEAM9_PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void MoveToNextNode(int DiceValue);

private:
	void UpdateMove();
	
	void OnDie();


public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	

	
private:
	FTimerHandle MoveTimerHandle;

	FVector MoveStart;

	FVector MoveTarget;

	float MoveSpeed;

	float MoveDuration;

	float MoveElapsed;

	int remainingMove;
};
