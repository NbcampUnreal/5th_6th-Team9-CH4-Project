// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TileManagerWolrdSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API UTileManagerWolrdSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

protected:

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> TilesData;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld);
protected:

private:
	void SpawnTiles();
};
