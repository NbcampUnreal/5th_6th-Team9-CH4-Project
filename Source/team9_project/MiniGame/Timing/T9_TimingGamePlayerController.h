#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "T9_TimingGamePlayerController.generated.h"

class UT9_TimingInGameWidget;

UCLASS()
class TEAM9_PROJECT_API AT9_TimingGamePlayerController : public AT9_MiniGamePlayerControllerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopTimer();

	virtual void BeginPlay() override;

	virtual void CreateUI() override;

	virtual void ShowInGameUI() override;

	virtual void RemoveAllUI() override;

	virtual void ChangeUI(EMiniGamePhase NewPhase) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UT9_TimingInGameWidget> InGameWidgetClass;

	UPROPERTY()
	TObjectPtr<UT9_TimingInGameWidget> InGameWidgetInstance;
};
