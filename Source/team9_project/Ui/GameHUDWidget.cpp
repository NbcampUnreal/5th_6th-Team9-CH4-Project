#include "GameHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_Dice)
    {
        Btn_Dice->OnClicked.AddDynamic(this, &UGameHUDWidget::OnDiceClicked);
    }

    if (Btn_ItemUse)
    {
        Btn_ItemUse->OnClicked.AddDynamic(this, &UGameHUDWidget::OnItemUseClicked);
    }

    UpdateHP(100.f, 100.f);
}

void UGameHUDWidget::UpdateHP(float CurrentHp, float MaxHp)
{
    if (HP_Bar && MaxHp > 0.f)
    {
        HP_Bar->SetPercent(CurrentHp / MaxHp);
    }

    if (Text_PlayerHP)
    {
        Text_PlayerHP->SetText(FText::FromString(TEXT("Player HP")));
    }
}

void UGameHUDWidget::OnDiceClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("[GameHUD] Dice button clicked"));
    BP_OnDiceButtonClicked();
}

void UGameHUDWidget::OnItemUseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("[GameHUD] Item button clicked"));
    BP_OnItemUseButtonClicked();
}