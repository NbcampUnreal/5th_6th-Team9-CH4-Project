#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_TimingInGameWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UT9_TimingInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnStopClicked();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountdownText;

	UPROPERTY(meta = (BindWidget))
	class UButton* StopButton;
};
