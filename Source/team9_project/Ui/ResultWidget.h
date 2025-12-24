#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/Border.h"
#include "Components/VerticalBoxSlot.h"
#include "ResultWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UResultWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Game Result")
    void SetupResults();

    UFUNCTION()
    void OnBackToMainClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_WinnerName;

    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* RankListContainer;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_BackToMain;

private:
    bool bResultsShown = false; // 중복 생성 방지
};