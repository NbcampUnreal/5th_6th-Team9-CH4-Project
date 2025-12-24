// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase_TileTarget.h"
#include "Effect_Teleporter.generated.h"

class ATile;
class ATileManagerActor;
class APlayerCharacter;

/**
 * 순간이동기 - 선택한 타일로 즉시 이동
 * - 조작 타입: TileTarget (A/D로 타일 순환 → Space로 확정)
 * - 효과: 선택한 타일로 즉시 이동
 */
UCLASS()
class TEAM9_PROJECT_API UEffect_Teleporter : public UItemEffectBase_TileTarget
{
	GENERATED_BODY()

public:
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;
};
