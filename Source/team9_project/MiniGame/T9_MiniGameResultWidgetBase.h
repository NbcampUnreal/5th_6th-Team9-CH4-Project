// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_MiniGameResultWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9_PROJECT_API UT9_MiniGameResultWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void OnResultScreenShown();

	virtual void SetWinnerText(const FText& InText);
};
