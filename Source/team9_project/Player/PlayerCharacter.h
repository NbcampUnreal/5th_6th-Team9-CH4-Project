// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponent;
class AMyPlayerState;
class ACameraPawn;
class ATile;
struct FTimerHandle;

UCLASS()
class TEAM9_PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void InitCharacter(ACameraPawn* InCameraPawn, AMyPlayerState* InPlyaerState);

	void MoveToNextNode(int DiceValue);

	void SetPlayerState(AMyPlayerState* InPlayerState);

	AMyPlayerState* GetPlayerState();

	ATile* GetCurrentTile();

	void SetCurrentTile(ATile* TileNode);

	bool OnDie();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCMove(int DiceValue);

private:
	void UpdateMove();

	

public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	
	int remainingMove;

private:
	UPROPERTY(Replicated)
	ACameraPawn* CameraPawn;

	UPROPERTY(Replicated)
	AMyPlayerState* MyPlayerState;

	UPROPERTY(Replicated, EditAnywhere)
	ATile* CurrentTile;

	// 타일 이동 변수
	FTimerHandle MoveTimerHandle;

	FVector MoveStart;

	FVector MoveTarget;

	float MoveSpeed;

	float MoveDuration;

	float MoveElapsed;
	//

	// 타일 인덱스
	int32 CurrentIndex;

	bool bIsMoving;
};
