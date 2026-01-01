#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_MiniGameReadyWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class TEAM9_PROJECT_API UT9_MiniGameReadyWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReadyButtonClicked();

	void SetGameNameText(const FText& NewText);

	void SetGameImageText(UTexture2D* NewTexture);

	void SetGameDescriptionText(const FText& NewText);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GameImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyButton;
};
