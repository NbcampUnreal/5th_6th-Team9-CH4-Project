// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/TileAction/ServerTileAction/ServerTileAction.h"
#include "DecreaseHPTileAction.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API UDecreaseHPTileAction : public UServerTileAction
{
	GENERATED_BODY()

public:
	virtual void Active(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	int32 Damage = 0;
};
