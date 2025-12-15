// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "TileData.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	_Mesh->SetupAttachment(GetRootComponent());
}

void ATile::AssignFromData(FTileData* data)
{
	if (data == nullptr) return;

	if (IsValid(GetRootComponent()) == false)
	{
		USceneComponent* Root = NewObject<USceneComponent>(this, TEXT("RootSceneComp"));
		Root->RegisterComponentWithWorld(GetWorld());
		SetRootComponent(Root);
	}

	AssignFromDataAsset(data->DataAsset);

	Index = data->Index;

	data->TileInstance = this;
}

//void ATile::SetConectedTiles(FTileData& data)
//{
//	
//}

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
}

