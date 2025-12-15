// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManagerWolrdSubsystem.h"
#include "TileData.h"
#include "Tile.h"

void UTileManagerWolrdSubsystem::OnWorldBeginPlay(UWorld& InWorld) 
{
	SpawnTiles();
}

void UTileManagerWolrdSubsystem::SpawnTiles()
{
    const FString DefaultPath = TEXT("/Game/Data/DataTable/TilesData.TilesData");
    UDataTable* datatable = LoadObject<UDataTable>(nullptr, *DefaultPath);
	checkf(IsValid(datatable), TEXT("DefaultPath is wrong, TileManagerWolrdSubsystem.cpp 16"));

	const TArray<FName> RowNames = datatable->GetRowNames();
	FString Context = TEXT("FTileData Lookup");

	int32 index = 0;
	for (const FName& RowName : RowNames)
	{
		FTileData* tiledata = datatable->FindRow<FTileData>(RowName, Context);
		tiledata->Index = index;
		if (!tiledata)
		{
			UE_LOG(LogTemp, Warning, TEXT("tiledata %s not found or invalid."), *RowName.ToString());
			continue;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(tiledata->Location);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATile* SpawnedTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), SpawnTransform, SpawnParams);

		if (!SpawnedTile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn tile for row %s"), *RowName.ToString());
			continue;
		}

		SpawnedTile->AssignFromData(tiledata);

		UE_LOG(LogTemp, Warning, TEXT("complete spawn Tile name : %s"), *RowName.ToString());

		index++;
	}
}
