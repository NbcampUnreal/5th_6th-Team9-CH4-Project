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

	void SetPlayerID(int Amount);

	int GetPlayerID();

	void SetHP(int Amount);

	int GetHP();

	void SetMaxHP(int Amount);

	int GetMaxHP();

	void AddHP(int Amount);

	void SetScore(int Amount);

	int GetScore();

	void AddScore(int Amount);

	void SetTileIndex(int Amount);

	int GetTileIndex();



private:
	int PlayerID;

	int CurrentHp;

	int MaxHp;

	int CurrentScore;

	int TileIndex;
};
