// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponent;
class AMyPlayerState;
class ACameraPawn;
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

	void InitCharacter(ACameraPawn* InCameraPawn);

	void MoveToNextNode(int DiceValue);

	void SetPlayerState(AMyPlayerState* InPlayerState);
	AMyPlayerState* GetPlayerState();

	bool OnDie();

private:
	void UpdateMove();

public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	int remainingMove;

private:
	UPROPERTY()
	ACameraPawn* CameraPawn;

	UPROPERTY()
	AMyPlayerState* MyPlayerState;


	FTimerHandle MoveTimerHandle;

	FVector MoveStart;

	FVector MoveTarget;

	float MoveSpeed;

	float MoveDuration;

	float MoveElapsed;

	int32 Currnet_index_Tile = 0;
};
