// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Effects/ItemEffectBase_TileTarget.h"
#include "Item/Data/ItemUseContext.h"
// Cho_SungMin - 타일 시스템 연동
#include "Tile/TileManagerActor.h"
#include "Tile/Tile.h"
#include "Player/PlayerCharacter.h"
#include "Player/CameraPawn.h"
#include "Item/TileSelectorActor.h"

void UItemEffectBase_TileTarget::StartUse(AActor* User)
{
	Super::StartUse(User);

	SelectedTileIndex = 0;
	InitializeTileList();

	// Cho_SungMin - 타일 셀렉터 스폰 및 위치 설정
	SpawnTileSelector();
	UpdateTileSelectorPosition();
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

	// Cho_SungMin - 효과 실행 후 타일 셀렉터 파괴
	DestroyTileSelector();
}

void UItemEffectBase_TileTarget::CancelUse()
{
	Super::CancelUse();
	SelectedTileIndex = 0;

	// Cho_SungMin - 타일 셀렉터 파괴
	DestroyTileSelector();
}

EItemUseType UItemEffectBase_TileTarget::GetUseType() const
{
	return EItemUseType::TileTarget;

}

void UItemEffectBase_TileTarget::CycleNextTile()
{
	if (TotalTileCount <= 0)
	{
		return;
	}

	// Cho_SungMin - 입력 쿨다운 체크
	UWorld* World = CurrentUser ? CurrentUser->GetWorld() : nullptr;
	float CurrentTime = World ? World->GetTimeSeconds() : 0.0f;
	if (CurrentTime - LastTileCycleTime < TileCycleCooldown)
	{
		return;
	}
	LastTileCycleTime = CurrentTime;

	SelectedTileIndex++;
	if (SelectedTileIndex >= TotalTileCount)
	{
		SelectedTileIndex = 0;
	}

	// Cho_SungMin - 셀렉터 위치 업데이트
	UpdateTileSelectorPosition();
}

void UItemEffectBase_TileTarget::CyclePrevTile()
{
	if (TotalTileCount <= 0)
	{
		return;
	}

	// Cho_SungMin - 입력 쿨다운 체크
	UWorld* World = CurrentUser ? CurrentUser->GetWorld() : nullptr;
	float CurrentTime = World ? World->GetTimeSeconds() : 0.0f;
	if (CurrentTime - LastTileCycleTime < TileCycleCooldown)
	{
		return;
	}
	LastTileCycleTime = CurrentTime;

	SelectedTileIndex--;
	if (SelectedTileIndex < 0)
	{
		SelectedTileIndex = TotalTileCount - 1;
	}

	// Cho_SungMin - 셀렉터 위치 업데이트
	UpdateTileSelectorPosition();
}

int32 UItemEffectBase_TileTarget::GetSelectedTileIndex() const
{
	return SelectedTileIndex;
}

// Cho_SungMin - 카메라 추적용 타일 셀렉터 위치 반환
FVector UItemEffectBase_TileTarget::GetTileSelectorLocation() const
{
	if (TileSelectorActor)
	{
		return TileSelectorActor->GetTargetLocation();
	}
	return FVector::ZeroVector;
}

void UItemEffectBase_TileTarget::UpdateContext(FItemUseContext& Context)
{
	Super::UpdateContext(Context);
	Context.TargetTileIndex = SelectedTileIndex;
}

void UItemEffectBase_TileTarget::InitializeTileList()
{
	if (!CurrentUser || !CurrentUser->GetWorld())
	{
		TotalTileCount = 0;
		return;
	}

	ATileManagerActor* TileManager = ATileManagerActor::Get(CurrentUser->GetWorld());
	if (!TileManager)
	{
		TotalTileCount = 0;
		return;
	}

	TotalTileCount = TileManager->GetTileCount();
	SelectedTileIndex = 0;

	ACameraPawn* CameraPawn = Cast<ACameraPawn>(CurrentUser);
	if (CameraPawn)
	{
		APlayerCharacter* PlayerChar = CameraPawn->GetPlayerCharacter();
		if (PlayerChar)
		{
			ATile* CurrentTile = PlayerChar->GetCurrentTile();
			if (CurrentTile)
			{
				SelectedTileIndex = CurrentTile->GetIndex();
			}
		}
	}

	if (SelectedTileIndex < 0 || SelectedTileIndex >= TotalTileCount)
	{
		SelectedTileIndex = 0;
	}
}

bool UItemEffectBase_TileTarget::CheckTimeout()
{
	return ElapsedTime >= TimeLimit;
}

void UItemEffectBase_TileTarget::SpawnTileSelector()
{
	if (TileSelectorActor)
	{
		TileSelectorActor->MultiRPC_SetVisible(true);
		return;
	}

	if (!CurrentUser || !CurrentUser->GetWorld())
	{
		return;
	}

	UWorld* World = CurrentUser->GetWorld();

	TSubclassOf<ATileSelectorActor> ClassToSpawn = TileSelectorClass;
	if (!ClassToSpawn)
	{
		ClassToSpawn = ATileSelectorActor::StaticClass();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TileSelectorActor = World->SpawnActor<ATileSelectorActor>(ClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}

void UItemEffectBase_TileTarget::DestroyTileSelector()
{
	if (TileSelectorActor)
	{
		TileSelectorActor->Destroy();
		TileSelectorActor = nullptr;
	}
}

void UItemEffectBase_TileTarget::UpdateTileSelectorPosition()
{
	if (!TileSelectorActor || !CurrentUser || !CurrentUser->GetWorld())
	{
		return;
	}

	ATileManagerActor* TileManager = ATileManagerActor::Get(CurrentUser->GetWorld());
	if (!TileManager)
	{
		return;
	}

	ATile* SelectedTile = TileManager->GetTile(SelectedTileIndex);
	if (SelectedTile)
	{
		TileSelectorActor->MultiRPC_SetTargetLocation(SelectedTile->GetActorLocation());
	}
}

