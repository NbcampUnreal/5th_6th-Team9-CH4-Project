// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_TileTarget.h"
#include "Effect_Beehive.generated.h"

class ATile;
class ATileManagerActor;
class APlayerCharacter;

/**
 * 벌꿀집 - 선택한 타일에 있는 모든 플레이어에게 데미지
 * - 조작 타입: TileTarget (A/D로 타일 순환 → Space로 확정)
 * - 효과: 선택한 타일에 있는 모든 플레이어에게 30 데미지
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_Beehive : public UItemEffectBase_TileTarget
{
	GENERATED_BODY()

public:
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;

protected:
	// 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beehive")
	float Damage = 30.0f;

	// 타일 범위 (타일 중심에서의 반경) - Source에서 추가됨
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beehive")
	float TileRadius = 150.0f;
};
