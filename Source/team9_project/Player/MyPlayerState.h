// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

	void SetHP(int Amount);

	int GetHP();

	void AddHP(int Amount);

	void SetScore(int Amount);

	int GetScore();

	void AddScore(int Amount);

public:
	int CurrentHp;

	int MaxHp;

	int CurrentScore;
};
