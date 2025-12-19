#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_MiniGameReadyWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class TEAM9_PROJECT_API UT9_MiniGameReadyWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReadyButtonClicked();


public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyButton;
};
