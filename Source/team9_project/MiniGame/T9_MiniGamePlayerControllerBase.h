#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "T9_MiniGamePlayerControllerBase.generated.h"

class UT9_MiniGameReadyWidget;
class UT9_MiniGameResultWidget;

enum class EMiniGamePhase : uint8;

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGamePlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	virtual void ServerRPCUpdatePlayerReady();

	virtual void BeginPlay() override;

	virtual void CreateUI();

	virtual void ShowReadyUI();

	virtual void ShowResultUI();

	virtual void ShowInGameUI();

	virtual void RemoveAllUI();

	virtual void ChangeUI(EMiniGamePhase NewPhase);

	virtual void SetInputEnabled(bool bEnable);

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UT9_MiniGameReadyWidget> ReadyWidgetClass;

	UPROPERTY()
	TObjectPtr<UT9_MiniGameReadyWidget> ReadyWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UT9_MiniGameResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<UT9_MiniGameResultWidget> ResultWidgetInstance;
};
