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

	int32 Winner() ;

	void OnTimeOver();

public:
	int32 TimerTime;

	float StartServerTime;

	FTimerHandle TimeOverHandle;

	int32 BestDeltaMs;
};
