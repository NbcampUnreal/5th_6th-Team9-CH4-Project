// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileManagerActor.h"
#include "TileData.h"
#include "Tile/Tile.h"

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

	////for (size_t i = 0; i < length; i++)
	////{

	////}

	//checkf(IsValid(_TilesData), TEXT("TilesData is Not Valid, ATileManagerActor::SpawnTiles"));

	//const TArray<FName> RowNames = _TilesData->GetRowNames();
	//FString Context = TEXT("FTileData Lookup");

	//int32 index = 0; 

	//for (size_t i = 0; i < RowNames.Num(); ++i)
	//{
	//	FTileData* tiledata = _TilesData->FindRow<FTileData>(RowNames[i], Context);

	//	if (!tiledata)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("tiledata %s not found or invalid."), *RowNames[i].ToString());
	//		continue;
	//	}

	//	FTransform SpawnTransform;
	//	SpawnTransform.SetLocation(tiledata->Location);

	//	FActorSpawnParameters SpawnParams;
	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	//SpawnParams.Name = RowNames[i];

	//	ATile* SpawnedTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), SpawnTransform, SpawnParams);
	//	/*SpawnedTile->GetName();
	//	SpawnedTile->Rename();*/

	//	if (!SpawnedTile)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn tile for row %s"), *RowNames[i].ToString());
	//		continue;
	//	}

	//	SpawnedTile->AssignFromData(tiledata, index);

	//	_Tiles.Add(SpawnedTile);

	//	UE_LOG(LogTemp, Warning, TEXT("complete spawn Tile name : %s"), *RowNames[i].ToString());

	//}


	////for (const FName& RowName : RowNames)
	////{
	////	
	////	index++;
	////}
}

void ATileManagerActor::LinkTiles()
{
	//checkf(IsValid(_TilesData), TEXT("TilesData is Not Valid, ATileManagerActor::LinkTiles"));
	////UE_LOG(LogTemp, Warning, TEXT("Nums : %d"), _Tiles.Num());
	////TilesData->GetRowStruct();

	////for (TWeakObjectPtr<ATile>& Tile : _Tiles)
	////{
	////	//Tile->
	////}



}
