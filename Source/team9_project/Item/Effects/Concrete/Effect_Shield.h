// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_Instant.h"
#include "Effect_Shield.generated.h"

/**
 * 실드 아이템 - 피해 1회 완전 무효화
 * - 조작 타입: Instant (즉시 발동)
 * - 효과: CountBased, 피해 1회 방어 후 소멸
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_Shield : public UItemEffectBase_Instant
{
	GENERATED_BODY()

public:
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;

protected:
	// 실드 지속 횟수 (기본값: 1회)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shield")
	int32 ShieldCount = 1;
};
