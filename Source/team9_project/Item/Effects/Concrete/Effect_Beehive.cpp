// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Beehive.h"
#include "Item/Data/ItemUseContext.h"
// #include "Tile/Tile.h"  // TODO: 타일 시스템 연동 시 활성화
// #include "Player/PlayerCharacter.h"  // TODO: 플레이어 시스템 연동 시 활성화
// #include "Kismet/GameplayStatics.h"
// #include "DrawDebugHelpers.h"

void UEffect_Beehive::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Beehive: User or World is null"));
		return;
	}

	// TODO: 타일/플레이어 시스템 연동
	// int32 TargetIndex = Context.TargetTileIndex;
	// if (TargetIndex < 0) { return; }
	// ATile* TargetTile = GetTileByIndex(TargetIndex);
	// if (!TargetTile) { return; }
	// TArray<AActor*> PlayersOnTile = GetPlayersOnTile(TargetTile);
	// for (AActor* Player : PlayersOnTile)
	// {
	//     if (!Player || Player == User) { continue; }
	//     FDamageEvent DamageEvent;
	//     Player->TakeDamage(Damage, DamageEvent, User->GetInstigatorController(), User);
	// }

	UE_LOG(LogTemp, Log, TEXT("Effect_Beehive: Effect triggered (damage not implemented yet)"));
}

// TODO: 타일/플레이어 시스템 연동 시 활성화
/*
TArray<ATile*> UEffect_Beehive::GetAllTiles() const
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

ATile* UEffect_Beehive::GetTileByIndex(int32 Index) const
{
	TArray<ATile*> AllTiles = GetAllTiles();

	if (Index >= 0 && Index < AllTiles.Num())
	{
		return AllTiles[Index];
	}

	return nullptr;
}

TArray<AActor*> UEffect_Beehive::GetPlayersOnTile(ATile* Tile) const
{
	TArray<AActor*> PlayersOnTile;

	if (!Tile || !CurrentUser || !CurrentUser->GetWorld())
	{
		return PlayersOnTile;
	}

	FVector TileLocation = Tile->GetActorLocation();

	// SphereOverlap으로 타일 범위 내 플레이어 찾기
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Tile); // 타일 자체는 무시

	bool bHasHit = CurrentUser->GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		TileLocation,
		FQuat::Identity,
		ECC_Pawn, // Pawn 채널 사용
		FCollisionShape::MakeSphere(TileRadius),
		QueryParams
	);

	// 결과에서 PlayerCharacter만 필터링
	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();
		if (Actor && Actor->IsA(APlayerCharacter::StaticClass()))
		{
			PlayersOnTile.Add(Actor);
		}
	}

	return PlayersOnTile;
}
*/
