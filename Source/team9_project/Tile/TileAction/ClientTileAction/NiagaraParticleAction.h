// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/TileAction/ClientTileAction/ClientTileAction.h"
#include "NiagaraParticleAction.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UCLASS()
class TEAM9_PROJECT_API UNiagaraParticleAction : public UClientTileAction
{
	GENERATED_BODY()
public:
	virtual void Active(APlayerCharacter* PlayerCharacter);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile|VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* NiagaraFX;
};
