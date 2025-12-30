#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_TimingInGameWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UT9_TimingInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnStopClicked();

	void UpdateRankingUI();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountdownText;

	UPROPERTY(meta = (BindWidget))
	class UButton* StopButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StopCheck;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOutCountdown;
};
