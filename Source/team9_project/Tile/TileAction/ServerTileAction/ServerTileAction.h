// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/TileAction/TileAction.h"
#include "ServerTileAction.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API UServerTileAction : public UTileAction
{
	GENERATED_BODY()

public:
	virtual void Active(APlayerCharacter* PlayerCharacter);
};
