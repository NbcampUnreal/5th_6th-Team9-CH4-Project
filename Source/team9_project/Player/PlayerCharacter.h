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

	void SetCharacterPosition();

	void SetPlayerState(AMyPlayerState* InPlayerState);

	AMyPlayerState* GetPlayerState();

	ATile* GetCurrentTile();

	void SetCurrentTile(ATile* TileNode);

	bool OnDie();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCMove(int DiceValue);

	// Cho_SungMin - 텔레포트 (NetMulticast로 모든 클라이언트에서 위치 동기화)
	// ATile*이 아닌 int32 TileIndex를 전달 (ATile은 서버/클라이언트가 각자 스폰하므로 포인터 공유 불가)
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Teleport(int32 TargetTileIndex);

private:
	void UpdateMove();

public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	
	int remainingMove;

	// 애니메이션 상태용
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Anim")
	bool bIsMoving;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Anim")
	bool bIsUsingItem;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Anim")
	bool bIsDie;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Anim")
	bool bIsHit;


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

	bool bPlayerLeave;
};
