#include "UIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UUIManagerSubsystem::SetUIState(EGameUIState NewState)
{
    // 기존 위젯 제거 
    ClearActiveWidgets();

    // None은 위젯 없이 입력모드만 변경
    if (NewState == EGameUIState::None)
    {
        CurrentState = NewState;
        UpdateInputMode(NewState);
        return;
    }

    // 위젯 클래스 존재 여부 + null 체크
    if (!UIWidgetMap.Contains(NewState) || !UIWidgetMap[NewState])
        return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    ActiveStateWidget = CreateWidget<UUserWidget>(PC, UIWidgetMap[NewState]);
    if (ActiveStateWidget)
    {
        ActiveStateWidget->AddToViewport();
        CurrentState = NewState;
        UpdateInputMode(NewState);
    }
}

void UUIManagerSubsystem::RegisterUIWidget(EGameUIState State, TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass)
    {
        UIWidgetMap.Add(State, WidgetClass);
    }
}

void UUIManagerSubsystem::StartHostGame(const FString& MapName)
{
    if (UWorld* World = GetWorld())
    {
        // ?listen 붙여서 listen 서버 생성 + Seamless Travel 자동 지원
        FString URL = MapName + TEXT("?listen");
        World->ServerTravel(URL);
    }
}

void UUIManagerSubsystem::StartJoinGame(const FString& IPAddress)
{
    if (UWorld* World = GetWorld())
    {
        FString Address = IPAddress.IsEmpty() ? TEXT("127.0.0.1") : IPAddress;
        GetWorld()->GetFirstPlayerController()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    }
}

void UUIManagerSubsystem::UpdateInputMode(EGameUIState State)
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    switch (State)
    {
    case EGameUIState::InGame:
        PC->SetInputMode(FInputModeGameAndUI());
        PC->bShowMouseCursor = true;
        break;
    case EGameUIState::None:
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
        break;
    default:
        PC->SetInputMode(FInputModeUIOnly());
        PC->bShowMouseCursor = true;
        break;
    }
}

void UUIManagerSubsystem::ClearActiveWidgets()
{
    if (ActiveStateWidget && ActiveStateWidget->IsInViewport())
    {
        ActiveStateWidget->RemoveFromParent();
        ActiveStateWidget = nullptr;
    }
}