// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Teleporter.h"
#include "Item/Data/ItemUseContext.h"
// #include "Tile/Tile.h"  // TODO: 타일 시스템 연동 시 활성화
// #include "Kismet/GameplayStatics.h"

void UEffect_Teleporter::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Teleporter: User is null"));
		return;
	}

	// TODO: 타일 시스템 연동
	// int32 TargetIndex = Context.TargetTileIndex;
	// if (TargetIndex < 0) { return; }
	// ATile* TargetTile = GetTileByIndex(TargetIndex);
	// if (!TargetTile) { return; }
	// FVector TeleportLocation = TargetTile->GetActorLocation();
	// TeleportLocation.Z += 100.0f;
	// User->SetActorLocation(TeleportLocation);

	UE_LOG(LogTemp, Log, TEXT("Effect_Teleporter: Effect triggered (teleport not implemented yet)"));
}

// TODO: 타일 시스템 연동 시 활성화
/*
TArray<ATile*> UEffect_Teleporter::GetAllTiles() const
{
	TArray<ATile*> FoundTiles;

	if (!CurrentUser || !CurrentUser->GetWorld())
	{
		return FoundTiles;
	}

	// 월드에서 모든 ATile 액터 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(CurrentUser->GetWorld(), ATile::StaticClass(), FoundActors);

	// ATile*로 캐스팅
	for (AActor* Actor : FoundActors)
	{
		if (ATile* Tile = Cast<ATile>(Actor))
		{
			FoundTiles.Add(Tile);
		}
	}

	return FoundTiles;
}

ATile* UEffect_Teleporter::GetTileByIndex(int32 Index) const
{
	TArray<ATile*> AllTiles = GetAllTiles();

	if (Index >= 0 && Index < AllTiles.Num())
	{
		return AllTiles[Index];
	}

	return nullptr;
}
*/
