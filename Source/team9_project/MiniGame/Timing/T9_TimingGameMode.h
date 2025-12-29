#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGameModeBase.h"
#include "T9_TimingGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API AT9_TimingGameMode : public AT9_MiniGameModeBase
{
	GENERATED_BODY()
	
public:
	AT9_TimingGameMode();

	virtual void BeginPlay() override;

	virtual void StartGame() override;

	virtual void EndGame() override;

	void Winner();

	void OnTimeOver();

public:
	float TimerTime;

	float StartServerTime;

	FTimerHandle TimeOverHandle;

	float BestDeltaMs;
};
