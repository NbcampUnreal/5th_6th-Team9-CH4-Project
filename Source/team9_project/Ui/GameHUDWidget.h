#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameHUDWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateHP(float CurrentHp, float MaxHp);
    void SetTurnTextVisibility(bool bIsMyTurn);

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HP_Bar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnAlert;
	
};
