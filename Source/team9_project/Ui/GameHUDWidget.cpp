// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/GameHUDWidget.h"

void UGameHUDWidget::UpdateHP(float CurrentHp, float MaxHp)
{
    if (HP_Bar && MaxHp > 0)
    {
        float Percent = CurrentHp / MaxHp;
        HP_Bar->SetPercent(Percent);
    }
}

void UGameHUDWidget::SetTurnTextVisibility(bool bIsMyTurn)
{
    if (Text_TurnAlert)
    {
        Text_TurnAlert->SetVisibility(bIsMyTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}