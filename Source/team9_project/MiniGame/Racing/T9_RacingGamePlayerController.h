#pragma once
#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "T9_RacingGamePlayerController.generated.h"

class UT9_RacingInGameWidget;

UCLASS()
class TEAM9_PROJECT_API AT9_RacingGamePlayerController : public AT9_MiniGamePlayerControllerBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void CreateUI() override;

	virtual void ShowInGameUI() override;

	virtual void RemoveAllUI() override;

	virtual void ChangeUI(EMiniGamePhase NewPhase) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UT9_RacingInGameWidget> InGameWidgetClass;

	UPROPERTY()
	TObjectPtr<UT9_RacingInGameWidget> InGameWidgetInstance;
};
