// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_Instant.h"
#include "Effect_BrokenTeleporter.generated.h"

/**
 * 망가진 순간이동기 - 랜덤 타일로 즉시 이동
 * - 조작 타입: Instant (즉시 발동)
 * - 효과: 맵의 랜덤 타일로 이동
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_BrokenTeleporter : public UItemEffectBase_Instant
{
	GENERATED_BODY()

public:
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;

protected:
	// TODO: 타일 시스템 연동 시 활성화
	// TArray<class ATile*> GetAllTiles() const;
};
