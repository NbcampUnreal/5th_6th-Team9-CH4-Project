// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManagerActor.generated.h"

class ATile;
struct FTileData;

UCLASS()
class TEAM9_PROJECT_API ATileManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileManagerActor();

protected:


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> _TilesData;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ATile>> _Tiles;

public:	

protected:
	virtual void BeginPlay() override;

private:
	void SpawnTiles();
	void LinkTiles();
	TArray<TWeakObjectPtr<ATile>> GetTilesByIndexes(TArray<int32>& Indexes) const ;
};
