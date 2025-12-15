// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "T9_MiniGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EMiniGamePhase : uint8
{
	Ready,
	Playing,
	Result
};

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AT9_MiniGameModeBase();

	virtual void BeginPlay() override;

	virtual void StartGame();

	virtual void EndGame();

	virtual bool CheckGame();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Phase)
	EMiniGamePhase CurrentPhase;

	void SetPhase(EMiniGamePhase NewPhase);

	UFUNCTION()
	void OnRep_Phase();

	virtual void OnPhaseChanged(EMiniGamePhase NewPhase);
};
