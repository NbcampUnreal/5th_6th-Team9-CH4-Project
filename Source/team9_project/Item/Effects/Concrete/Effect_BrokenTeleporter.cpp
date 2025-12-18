// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_BrokenTeleporter.h"
#include "Item/Data/ItemUseContext.h"
// #include "Tile/Tile.h"  // TODO: 타일 시스템 연동 시 활성화
// #include "Kismet/GameplayStatics.h"

void UEffect_BrokenTeleporter::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_BrokenTeleporter: User is null"));
		return;
	}

	// TODO: 타일 시스템 연동
	// TArray<ATile*> AllTiles = GetAllTiles();
	// if (AllTiles.Num() == 0) { return; }
	// int32 RandomIndex = FMath::RandRange(0, AllTiles.Num() - 1);
	// ATile* RandomTile = AllTiles[RandomIndex];
	// if (!RandomTile) { return; }
	// FVector TeleportLocation = RandomTile->GetActorLocation();
	// TeleportLocation.Z += 100.0f;
	// User->SetActorLocation(TeleportLocation);

	UE_LOG(LogTemp, Log, TEXT("Effect_BrokenTeleporter: Effect triggered (teleport not implemented yet)"));
}

// TODO: 타일 시스템 연동 시 활성화
/*
TArray<ATile*> UEffect_BrokenTeleporter::GetAllTiles() const
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
*/
