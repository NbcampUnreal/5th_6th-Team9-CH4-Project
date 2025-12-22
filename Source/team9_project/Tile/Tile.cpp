// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "TileData.h"
#include "Player/PlayerCharacter.h"
#include "TileComponent.h"

// Sets default values
ATile::ATile()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(_Root);

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	_Mesh->SetupAttachment(GetRootComponent());
	
	PrimaryActorTick.bCanEverTick = false;
}

void ATile::AssignFromData(FTileData* data, int32 index)
{
	if (data == nullptr) return;

	AssignFromDataAsset(data->DataAsset);

	_Index = index;
}

void ATile::SetLinkTiles(const TArray<TWeakObjectPtr<ATile>>& BeforeTile, const TArray<TWeakObjectPtr<ATile>>& NextTile)
{
	_BeforeTile = BeforeTile;
	_NextTile = NextTile;
}

void ATile::PlayerArrive(APlayerCharacter* PlayerCharacter)
{
	_InPlayers.Add(PlayerCharacter);
	OnPlayerArrive.Broadcast(PlayerCharacter);
}

void ATile::PlayerPassed(APlayerCharacter* PlayerCharacter)
{
	OnPlayerPassed.Broadcast(PlayerCharacter);
}

void ATile::PlayerLeave(APlayerCharacter* PlayerCharacter)
{
	OnPlayerLeave.Broadcast(PlayerCharacter);
	_InPlayers.Remove(PlayerCharacter);
}

void ATile::PlayerUseItem(APlayerCharacter* PlayerCharacter)
{
	OnPlayerUseItem.Broadcast(PlayerCharacter);
}

void ATile::PlayerRollDice(APlayerCharacter* PlayerCharacter)
{
	OnPlayerRollDice.Broadcast(PlayerCharacter);
}

void ATile::ServerRPC_PlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
}

void ATile::ServerRPC_PlayerPassed_Implementation(APlayerCharacter* PlayerCharacter)
{
}

void ATile::ServerRPC_PlayerArrive_Implementation(APlayerCharacter* PlayerCharacter)
{
}

void ATile::ServerRPC_PlayerUseItem_Implementation(APlayerCharacter* PlayerCharacter)
{
}

void ATile::ServerRPC_PlayerRollDice_Implementation(APlayerCharacter* PlayerCharacter)
{
}

TArray<APlayerCharacter*> ATile::GetInPlayers()
{
	TArray<APlayerCharacter*> Result;
	for (TWeakObjectPtr<APlayerCharacter> Character: _InPlayers)
	{
		if (Character.IsValid())
		{
			Result.Add(Character.Get());
		}
	}

	return Result;
}

TArray<ATile*> ATile::GetNextTiles()
{
	TArray<ATile*> Result;

	for (auto Tile : _NextTile)
	{
		if (Tile.IsValid())
		{
			Result.Add(Tile.Get());
		}
	}

	return Result;
}

TArray<ATile*> ATile::GetBeforeTiles()
{
	TArray<ATile*> Result;

	for (auto Tile : _BeforeTile)
	{
		if (Tile.IsValid())
		{
			Result.Add(Tile.Get());
		}
	}

	return Result;
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::AssignFromDataAsset(UTileDataAsset* asset)
{
	if (IsValid(asset) == false) return;
	if (IsValid(_Mesh) == false) return;

	if (IsValid(asset->Mesh)) {
		_Mesh->SetStaticMesh(asset->Mesh);
	}

	TArray<TSubclassOf<UTileComponent>>& TileComponents = asset->TileComponents;

	for (const TSubclassOf<UTileComponent>& SubClass : TileComponents)
    {
        if (!SubClass) continue;

        UTileComponent* NewComp = NewObject<UTileComponent>(this, SubClass);
        if (!NewComp) continue;

		NewComp->RegisterComponent(); 
		_TileComponents.Add(NewComp);
    }
}

