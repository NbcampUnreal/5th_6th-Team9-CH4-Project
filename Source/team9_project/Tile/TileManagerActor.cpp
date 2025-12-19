// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileManagerActor.h"
#include "TileData.h"
#include "Tile/Tile.h"

TWeakObjectPtr<ATileManagerActor> ATileManagerActor::SingletonInstance = nullptr;

// Sets default values
ATileManagerActor::ATileManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATileManagerActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnTiles();
	LinkTiles();
}

void ATileManagerActor::SpawnTiles(){
	checkf(IsValid(_TilesData), TEXT("TilesData is Not Valid, ATileManagerActor::SpawnTiles"));

	const int32 Count = _TilesData->GetRowNames().Num();
	FString Context = TEXT("FTileData Lookup");

	for (size_t i = 0; i < Count; ++i)
	{
		FName RowName(FString::Printf(TEXT("%d"), i));
		FTileData* Tiledata = _TilesData->FindRow<FTileData>(RowName, Context);

		if (!Tiledata)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tiledata's index : %d is not found or invalid."), i);
			continue;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Tiledata->Location);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATile* SpawnedTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), SpawnTransform, SpawnParams);

		if (!SpawnedTile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn tile for row index : %d"), i);
			continue;
		}

		_Tiles.Add(SpawnedTile);

		SpawnedTile->AssignFromData(Tiledata, i);
		UE_LOG(LogTemp, Warning, TEXT("complete spawn Tile index : %d"), i);
	}
}

void ATileManagerActor::LinkTiles()
{
	checkf(IsValid(_TilesData), TEXT("TilesData is Not Valid, ATileManagerActor::LinkTiles"));

	const int32 Count = _Tiles.Num();
	FString Context = TEXT("FTileData Lookup");

	ATile* TempTile;
	for (size_t i = 0; i < Count; ++i)
	{
		FName RowName(FString::Printf(TEXT("%d"), i));
		FTileData* Tiledata = _TilesData->FindRow<FTileData>(RowName, Context);

		if (!Tiledata)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tiledata's index : %d is not found or invalid."), i);
			continue;
		}

		if (_Tiles[i].IsValid())
		{
			TempTile = _Tiles[i].Get();

			TArray<TWeakObjectPtr<ATile>> BeforeTilesIndex = GetTilesByIndexes(Tiledata->BeforeTilesIndex);
			TArray<TWeakObjectPtr<ATile>> NextTilesIndex = GetTilesByIndexes(Tiledata->NextTilesIndex);

			TempTile->SetLinkTiles(BeforeTilesIndex, NextTilesIndex);
		}
	}
}

TArray<TWeakObjectPtr<ATile>> ATileManagerActor::GetTilesByIndexes(TArray<int32>& Indexes) const
{
	TArray<TWeakObjectPtr<ATile>> result;

	for (const int32 index : Indexes)
	{
		if (_Tiles[index].IsValid())
		{
			result.Add(_Tiles[index]);
		}
	}

	return result;
}
