// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AimIndicatorActor.generated.h"

class UDecalComponent;
class USceneComponent;

UCLASS()
class TEAM9_PROJECT_API AAimIndicatorActor : public AActor
{
	GENERATED_BODY()

public:
	AAimIndicatorActor();

	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 조준 방향 설정
	UFUNCTION(BlueprintCallable)
	void SetAimDirection(FVector Direction);

	// 범위 설정 (각도, 거리)
	UFUNCTION(BlueprintCallable)
	void SetIndicatorSize(float Angle, float Range);

	// 따라다닐 타겟 설정
	UFUNCTION(BlueprintCallable)
	void SetFollowTarget(AActor* Target);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDecalComponent* DecalComp;

	// Replicated 조준 방향
	UPROPERTY(ReplicatedUsing=OnRep_AimDirection)
	FVector AimDirection = FVector::ForwardVector;

	UPROPERTY()
	AActor* FollowTarget = nullptr;

	// 오프셋 (캐릭터 발 밑)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HeightOffset = 5.0f;

	UFUNCTION()
	void OnRep_AimDirection();

	void UpdateRotation();
	void UpdatePosition();
};
