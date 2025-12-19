#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "T9_MiniGameModeBase.generated.h"

enum class EMiniGamePhase : uint8;

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AT9_MiniGameModeBase();

	virtual void BeginPlay() override;

	virtual void StartGame();

	virtual void EndGame();

	virtual bool CheckPlayer();

	virtual void CheckStartGame();

	void ComeBackMainGame();

protected:

	void SetPhase(EMiniGamePhase NewPhase);

	FTimerHandle ComeBackHandle;
};
