#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUDWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // 버튼 클릭 핸들러 (C++ 내부용)
    UFUNCTION()
    void OnDiceClicked();

    UFUNCTION()
    void OnItemUseClicked();

public:
    // 블루프린트에서 구현할 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void BP_OnDiceButtonClicked();

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void BP_OnItemUseButtonClicked();

    // HP 갱신
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHP(float CurrentHp, float MaxHp);

    // BindWidget
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HP_Bar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_PlayerHP;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_Dice;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_ItemUse;
};