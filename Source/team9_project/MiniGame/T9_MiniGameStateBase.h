// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "T9_MiniGameStateBase.generated.h"

USTRUCT(BlueprintType)
struct FMiniGamePlayerResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerId = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 Score = 0;
};

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void AddPlayerResult(int32 PlayerId, int32 Score);

public:
	UPROPERTY(Replicated)
	TArray<FMiniGamePlayerResult> PlayerResults;

	bool bInGame = false;
};
