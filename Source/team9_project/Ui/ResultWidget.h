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
    // 서버 점수 기준으로 순위 계산 및 UI 표시 (외부에서 호출)
    UFUNCTION(BlueprintCallable, Category = "Game Result")
    void SetupResults();

    UFUNCTION()
    void OnBackToMainClicked();

private:
    // 순위 행 UI 생성 (내부 함수)
    void CreateRankRow(int32 Rank, const FString& PlayerName, int32 Score, UPanelWidget* Container);

    // 중복 생성 방지
    bool bResultsShown = false;

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_WinnerName;  // TODO: 1위 이름 강조

    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* RankListContainer;  // VerticalBox로 순위 리스트

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_BackToMain;  // 메인으로 돌아가기 버튼
};