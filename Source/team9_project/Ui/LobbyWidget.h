// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void UpdatePlayerList();

    UFUNCTION()
    void OnActionClicked();

public:
    UPROPERTY(meta = (BindWidget))
    UPanelWidget* PlayerListContainer; // ScrollBox or VerticalBox

    UPROPERTY(meta = (BindWidget))
    USpinBox* RoundSettingSpinner;

    UPROPERTY(meta = (BindWidget))
    UButton* Btn_Action;

    // UMG에서 슬롯 생성을 위해 할당 필요
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> LobbySlotClass;
};
