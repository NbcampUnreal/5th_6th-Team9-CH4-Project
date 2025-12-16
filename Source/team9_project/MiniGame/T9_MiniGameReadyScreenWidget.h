// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_MiniGameReadyScreenWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class TEAM9_PROJECT_API UT9_MiniGameReadyScreenWidget : public UUserWidget
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
