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

	virtual void CopyProperties(APlayerState* NewPlayerState) override;

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

	int32 GetPlayerId();

	void SetPlayerId(int32 PId);

private:
	int CurrentHp;

	int MaxHp;

	int CurrentScore;

	int TileIndex;

	int32 PlayerId;
};
