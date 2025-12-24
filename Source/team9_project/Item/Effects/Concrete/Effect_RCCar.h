// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_DirectControl.h"
#include "Effect_RCCar.generated.h"

/**
 * RC카 - WASD로 직접 조작 후 Space로 폭발
 * - 조작 타입: DirectControl (WASD 이동 → Space로 발동)
 * - 효과: RC카를 소환하여 30초간 조작, Space 키로 폭발
 *         폭발 시 1타일 범위 내 모든 플레이어에게 50 데미지
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_RCCar : public UItemEffectBase_DirectControl
{
	GENERATED_BODY()

public:
	virtual AActor* SpawnControlledActor(AActor* User) override;
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;

protected:
	// RC카 클래스 (블루프린트에서 설정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RCCar")
	TSubclassOf<AActor> RCCarClass;

	// 폭발 반경 (1타일 범위)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RCCar")
	float ExplosionRadius = 300.0f;

	// 폭발 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RCCar")
	float ExplosionDamage = 50.0f;
};
