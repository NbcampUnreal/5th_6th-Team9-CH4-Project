// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_MouseAim.h"
#include "Effect_BaseballBat.generated.h"

/**
 * 야구방망이 - 마우스 방향 근접 범위 공격
 * - 조작 타입: MouseAim (마우스 조준 → 좌클릭)
 * - 효과: 마우스 방향 1타일 거리 내 모든 플레이어에게 50 데미지
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_BaseballBat : public UItemEffectBase_MouseAim
{
	GENERATED_BODY()

public:
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;

protected:
	// 공격 범위 (1타일 거리)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseballBat")
	float AttackRange = 300.0f;

	// 공격 반경 (구체 범위)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseballBat")
	float AttackRadius = 200.0f;

	// 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseballBat")
	float Damage = 50.0f;
};
