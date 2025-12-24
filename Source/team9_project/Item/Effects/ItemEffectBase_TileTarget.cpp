// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Effects/ItemEffectBase_TileTarget.h"
#include "Item/Data/ItemUseContext.h"

void UItemEffectBase_TileTarget::StartUse(AActor* User)
{
	Super::StartUse(User);

	SelectedTileIndex = 0;
	InitializeTileList();
}

void UItemEffectBase_TileTarget::TickUse(float DeltaTime)
{
	Super::TickUse(DeltaTime);
	if (!bIsOperating)
	{
		return;
	}
	if (CheckTimeout())
	{
		CancelUse();
		return;
	}

}

void UItemEffectBase_TileTarget::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);


}

void UItemEffectBase_TileTarget::CancelUse()
{
	Super::CancelUse();
	SelectedTileIndex = 0;

}

EItemUseType UItemEffectBase_TileTarget::GetUseType() const
{
	return EItemUseType::TileTarget;

}

void UItemEffectBase_TileTarget::CycleNextTile()
{
	if (TotalTileCount <=0)
	{
		return;
	}
	SelectedTileIndex++;
	if (SelectedTileIndex >= TotalTileCount)
	{
		SelectedTileIndex = 0;
	}
}

void UItemEffectBase_TileTarget::CyclePrevTile()
{
	if (TotalTileCount <= 0)
	{
		return;
	}

	SelectedTileIndex--;
	if (SelectedTileIndex < 0)
	{
		SelectedTileIndex = TotalTileCount - 1;
	}
}

int32 UItemEffectBase_TileTarget::GetSelectedTileIndex() const
{
	return SelectedTileIndex;
}

void UItemEffectBase_TileTarget::UpdateContext(FItemUseContext& Context)
{
	Super::UpdateContext(Context);
	Context.TargetTileIndex = SelectedTileIndex;
}

void UItemEffectBase_TileTarget::InitializeTileList()
{
	// TODO: 타일 시스템 연결 후 구현
	// 1. 타일 매니저에서 전체 타일 목록 가져오기
	// 2. TotalTileCount 설정

	// TODO: 플레이어 시스템 연결 후 구현
	// SelectedTileIndex를 아이템 사용자의 현재 타일로 초기화
	// 예: SelectedTileIndex = GetPlayerCurrentTileIndex(CurrentUser);
	TotalTileCount = 10;
}

bool UItemEffectBase_TileTarget::CheckTimeout()
{
	return ElapsedTime >= TimeLimit;
}

