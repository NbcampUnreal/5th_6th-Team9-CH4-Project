// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileDataAsset.generated.h"

/**
 * 
 */

class UTileComponent;

UCLASS()
class TEAM9_PROJECT_API UTileDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<TSubclassOf<UTileComponent>> TileComponents;

protected:

private:

public:

protected:

private:

};
