// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/TileAction/ServerTileAction/ServerTileAction.h"
#include "GainScoreTileAction.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API UGainScoreTileAction : public UServerTileAction
{
	GENERATED_BODY()

public:
	virtual void Active(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;
};
