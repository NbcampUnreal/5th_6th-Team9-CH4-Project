// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API ATestMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATestMainGameMode();

	FTimerHandle TimerHandle_DelayedAction;

	UFUNCTION()
	void ExecuteAfter5Seconds();

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	int32 PlayerNumber;

	UPROPERTY()
	TSet<AController*> PlayersInLobby;

	int32 GivePlayerNumber();
};
