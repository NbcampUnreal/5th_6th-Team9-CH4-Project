// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileAction.generated.h"

/**
 * 
 */

class APlayerCharacter;

UCLASS(BlueprintType, Blueprintable)
class TEAM9_PROJECT_API UTileAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void Active(APlayerCharacter* PlayerCharacter);
protected:

private:

public:
	
protected:

private:
};
