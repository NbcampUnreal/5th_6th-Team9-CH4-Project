// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGameModeBase.h"
#include "T9_RacingGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API AT9_RacingGameMode : public AT9_MiniGameModeBase
{
	GENERATED_BODY()
	
public:
	AT9_RacingGameMode();

	virtual void BeginPlay() override;

	virtual void StartGame() override;

	virtual void EndGame() override;
};
