// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "T9_MiniGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API AT9_MiniGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void StartGame();

	virtual void EndGame();

	virtual bool CheckGame();
};
