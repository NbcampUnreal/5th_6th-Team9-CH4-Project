// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_MouseAim.h"
#include "Effect_Shotgun.generated.h"

/**
 * 샷건 - 마우스 방향으로 부채꼴 다중 총알 발사
 * - 조작 타입: MouseAim (마우스 조준 → 좌클릭)
 * - 효과: 부채꼴 범위로 여러 총알 발사, 총알당 10 데미지
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_Shotgun : public UItemEffectBase_MouseAim
{
	GENERATED_BODY()

public:
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;

protected:
	// 발사할 총알 개수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shotgun")
	int32 BulletCount = 5;

	// 부채꼴 펼침 각도 (한쪽 방향, 총 각도는 이것의 2배)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shotgun")
	float SpreadAngle = 15.0f;

	// 총알 사거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shotgun")
	float Range = 2000.0f;

	// 총알당 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shotgun")
	float DamagePerBullet = 10.0f;

	// 단일 총알 발사 (LineTrace)
	void FireBullet(AActor* User, const FVector& Direction);
};
