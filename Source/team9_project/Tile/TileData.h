// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TileDataAsset.h"
#include "TileData.generated.h"

/**
 * 
 */

class UStaticMesh;
class ATile;

USTRUCT(BlueprintType)
struct FTileData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TObjectPtr<UTileDataAsset> DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<int32> BeforeTilesIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<int32> NextTilesIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FVector Location;
};
