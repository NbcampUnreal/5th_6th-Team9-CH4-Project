#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API ULobbyWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override; // 위젯이 사라질 때 타이머 해제용
    bool bGameStarted = false;

public:
    UFUNCTION(BlueprintCallable)
    void UpdatePlayerList();

    UFUNCTION()
    void OnActionClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* PlayerListContainer;

    UPROPERTY(meta = (BindWidget))
    class USpinBox* RoundSettingSpinner;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_Action;

private:
    FTimerHandle RefreshTimerHandle; // 리스트 갱신용 타이머
};