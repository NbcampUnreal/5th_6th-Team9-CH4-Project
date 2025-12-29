// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileManagerActor.h"
#include "TileData.h"
#include "Tile/Tile.h"
#include "Player/PlayerCharacter.h"

TWeakObjectPtr<ATileManagerActor> ATileManagerActor::SingletonInstance = nullptr;

// Sets default values
ATileManagerActor::ATileManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

ATile* ATileManagerActor::GetTile(int32 Index)
{
	ATile* Result = nullptr;

	if (_Tiles[Index].IsValid())
	{
		Result = _Tiles[Index].Get();
	}
	return Result;
}

void ATileManagerActor::ServerRPC_PlayerArrive_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	if (HasAuthority() == false) return;

	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		NetMulticastRPC_PlayerArrive(TileIndex, PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::ServerRPC_PlayerPassed_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	if (HasAuthority() == false) return;

	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		NetMulticastRPC_PlayerPassed(TileIndex, PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::ServerRPC_PlayerLeave_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	if (HasAuthority() == false) return;

	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		NetMulticastRPC_PlayerLeave(TileIndex, PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::ServerRPC_PlayerUseItem_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	if (HasAuthority() == false) return;

	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		NetMulticastRPC_PlayerUseItem(TileIndex, PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::ServerRPC_PlayerRollDice_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	if (HasAuthority() == false) return;

	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		NetMulticastRPC_PlayerRollDice(TileIndex, PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::PlayerArrive(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	Tile->PlayerArrive(PlayerCharacter);

	//IsServer
	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		ServerRPC_PlayerArrive(TileIndex, PlayerCharacter);
	}
}

void ATileManagerActor::PlayerPassed(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	Tile->PlayerPassed(PlayerCharacter);

	//IsServer
	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		ServerRPC_PlayerPassed(TileIndex, PlayerCharacter);
	}
}

void ATileManagerActor::PlayerLeave(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	Tile->PlayerLeave(PlayerCharacter);

	//IsServer
	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		ServerRPC_PlayerLeave(TileIndex, PlayerCharacter);
	}
}

void ATileManagerActor::PlayerUseItem(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	Tile->PlayerUseItem(PlayerCharacter);
	
	//IsServer
	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		PlayerUseItem(TileIndex, PlayerCharacter);
	}
}

void ATileManagerActor::PlayerRollDice(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	Tile->PlayerRollDice(PlayerCharacter);

	//IsServer
	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		ServerRPC_PlayerRollDice(TileIndex, PlayerCharacter);
	}
}

// Called when the game starts or when spawned
void ATileManagerActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnTiles();
	LinkTiles();

	SingletonInstance = this;
	/*if (IsValid(SingletonInstance.Get()) == false)
	{
		SingletonInstance = this;
	}*/
}

void ATileManagerActor::SpawnTiles() {
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

		SpawnedTile->AssignFromData(Tiledata, i);

		UE_LOG(LogTemp, Warning, TEXT("complete spawn Tile index : %d"), i);

		_Tiles.Add(SpawnedTile);
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

void ATileManagerActor::NetMulticastRPC_PlayerRollDice_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		Tile->PlayerRollDice(PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::NetMulticastRPC_PlayerUseItem_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		Tile->PlayerUseItem(PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::NetMulticastRPC_PlayerLeave_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		Tile->PlayerLeave(PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::NetMulticastRPC_PlayerPassed_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		Tile->PlayerPassed(PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}

void ATileManagerActor::NetMulticastRPC_PlayerArrive_Implementation(int32 TileIndex, APlayerCharacter* PlayerCharacter)
{
	ATile* Tile = GetTile(TileIndex);
	if (IsValid(Tile) && IsValid(PlayerCharacter)) {
		Tile->PlayerArrive(PlayerCharacter);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Tile or PlayerCharacter is not Valid"));
	}
}
