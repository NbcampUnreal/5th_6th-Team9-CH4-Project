#include "UIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GenericPlatform/GenericPlatformHttp.h"

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

void UUIManagerSubsystem::StartHostGame(const FString& MapName, const FString& PlayerName)
{
    if (UWorld* World = GetWorld())
    {
        //MapName?listen?Name=플레이어이름
        FString URL = FString::Printf(TEXT("%s?listen?Name=%s"), *MapName, *PlayerName);
        UE_LOG(LogTemp, Warning, TEXT("Host URL: %s"), *URL);
        World->ServerTravel(URL);
    }
}

void UUIManagerSubsystem::StartJoinGame(const FString& IPAddress, const FString& PlayerName)
{
    if (UWorld* World = GetWorld())
    {
        FString Address = IPAddress.IsEmpty() ? TEXT("127.0.0.1") : IPAddress;
        //IP주소?Name=플레이어이름
        FString URL = FString::Printf(TEXT("%s?Name=%s"), *Address, *PlayerName);
        UE_LOG(LogTemp, Warning, TEXT("Join URL: %s"), *URL);
        GetWorld()->GetFirstPlayerController()->ClientTravel(URL, ETravelType::TRAVEL_Absolute);

        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            PC->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
        }
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