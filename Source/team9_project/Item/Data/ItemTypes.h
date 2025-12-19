#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

// 아이템 카테고리
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Attack    UMETA(DisplayName = "Attack"),
	Buff      UMETA(DisplayName = "Buff"),
	Movement  UMETA(DisplayName = "Movement")
};

// 아이템 조작 타입
UENUM(BlueprintType)
enum class EItemUseType : uint8
{
	Instant        UMETA(DisplayName = "Instant"),
	MouseAim       UMETA(DisplayName = "MouseAim"),
	TileTarget     UMETA(DisplayName = "TileTarget"),
	DirectControl  UMETA(DisplayName = "DirectControl")
};

// 버프/디버프 종료 조건
UENUM(BlueprintType)
enum class EEffectEndCondition : uint8
{
	RoundBased  UMETA(DisplayName = "RoundBased"),
	CountBased  UMETA(DisplayName = "CountBased")
};