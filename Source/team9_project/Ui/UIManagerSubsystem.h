#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Ui/EGameUIState.h"
#include "UIManagerSubsystem.generated.h"

class UUserWidget;

UCLASS(Blueprintable)
class TEAM9_PROJECT_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void SetUIState(EGameUIState NewState);
    void RegisterUIWidget(EGameUIState State, TSubclassOf<UUserWidget> WidgetClass);

    void StartHostGame(const FString& MapName, const FString& PlayerName);
    void StartJoinGame(const FString& IPAddress, const FString& PlayerName);

    template<typename T = UUserWidget>
    T* GetActiveWidget() const { return Cast<T>(ActiveStateWidget); }

    void ClearActiveWidgets();
    void ReturnToMainTitle();

protected:
    UPROPERTY()
    EGameUIState CurrentState = EGameUIState::None;

    UPROPERTY()
    UUserWidget* ActiveStateWidget = nullptr;

    TMap<EGameUIState, TSubclassOf<UUserWidget>> UIWidgetMap;

    UPROPERTY(EditAnywhere, Category = "Map to UI Mapping")
    TMap<FString, EGameUIState> MapKeywordToUIState;

private:
    void UpdateInputMode(EGameUIState State);

    // 타이머 핸들
    FTimerHandle MapCheckTimerHandle;

    // 파라미터 없는 함수로 변경 (타이머 바인딩용)
    void CheckAndSetUIStateForCurrentMap();
};