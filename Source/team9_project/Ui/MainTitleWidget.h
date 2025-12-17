// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "MainTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API UMainTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateGameClicked();

	UFUNCTION()
	void OnJoinGameClicked();

	UFUNCTION()
	void OnQuitClicked();

public:
	UPROPERTY(meta = (BindWidget))
	UEditableText* Input_Name;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_CreateGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_JoinGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Quit;
};
