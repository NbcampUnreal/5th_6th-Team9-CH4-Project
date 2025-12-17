// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGamePawnBase.h"
#include "T9_RacingGamePawn.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API AT9_RacingGamePawn : public AT9_MiniGamePawnBase
{
	GENERATED_BODY()
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
