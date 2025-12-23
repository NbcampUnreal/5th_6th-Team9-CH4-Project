#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Ui/EGameUIState.h"
#include "UIManagerSubsystem.generated.h"

UCLASS()
class TEAM9_PROJECT_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    void SetUIState(EGameUIState NewState);
    void RegisterUIWidget(EGameUIState State, TSubclassOf<UUserWidget> WidgetClass);

    // FString으로 통일 (FName → 에러 방지)
    void StartHostGame(const FString& MapName, const FString& PlayerName);
    void StartJoinGame(const FString& IPAddress, const FString& PlayerName);
    //void StartJoinGame(const FString& IPAddress = TEXT("127.0.0.1"));

    template<typename T = UUserWidget>
    T* GetActiveWidget() const { return Cast<T>(ActiveStateWidget); }

    void ClearActiveWidgets();

protected:
    UPROPERTY()
    EGameUIState CurrentState = EGameUIState::None;

    UPROPERTY()
    UUserWidget* ActiveStateWidget = nullptr;

    TMap<EGameUIState, TSubclassOf<UUserWidget>> UIWidgetMap;

private:
    void UpdateInputMode(EGameUIState State);
};