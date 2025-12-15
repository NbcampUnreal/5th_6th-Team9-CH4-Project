#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "T9_MiniGamePlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API AT9_MiniGamePlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void ShowReadyScreen();

	virtual void ShowResultScreen();

	virtual void ClearWidget();

	virtual void SetInputEnabled(bool bEnable);

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;
};
