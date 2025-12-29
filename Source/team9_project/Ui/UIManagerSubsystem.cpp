#include "UIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Ui/EGameUIState.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

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

void UUIManagerSubsystem::ReturnToMainTitle()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // 호스트만 ServerTravel 호출 (모든 클라이언트 이동)
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        if (PC->HasAuthority())
        {
            // 메인 타이틀 맵 경로
            FString MainTitleMap = TEXT("/Game/KJH/Test/MainTitleLevel");
            World->ServerTravel(MainTitleMap);
            UE_LOG(LogTemp, Warning, TEXT("[UIManager] All Player Move To MainTitle: %s"), *MainTitleMap);
        }
    }

    // UI 상태 초기화
    SetUIState(EGameUIState::MainMenu);
}

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 맵 로드 완료 델리게이트 바인딩
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(MapCheckTimerHandle,this,&UUIManagerSubsystem::CheckAndSetUIStateForCurrentMap,0.5f,false);
    }
    // === 임시 테스트용 Result 위젯 직접 등록 ===
    TSubclassOf<UUserWidget> ResultWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/KJH/WBP_ResultWidget.WBP_ResultWidget_C"));    if (ResultWidgetClass)
    {
        RegisterUIWidget(EGameUIState::Result, ResultWidgetClass);
        UE_LOG(LogTemp, Log, TEXT("[UIManager] ResultWidget Success!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UIManager] ResultWidget Surch Fail: /All/Game/KJH/WBP_ResultWidget"));
    }
    // === 임시 인벤토리 위젯 HUD 겹쳐서 나오게하기 ===
    TSubclassOf<UUserWidget> InventoryWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/WBP_Inventory.WBP_Inventory_C"));
    if (InventoryWidgetClass)
    {
        RegisterUIWidget(EGameUIState::Inventory, InventoryWidgetClass);  // 새로운 상태 추가 필요
        UE_LOG(LogTemp, Log, TEXT("[UIManager] InventoryWidget Registered"));
    }
}

void UUIManagerSubsystem::Deinitialize()
{
    // 타이머 정리
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MapCheckTimerHandle);
    }

    ClearActiveWidgets();
    Super::Deinitialize();
}

void UUIManagerSubsystem::CheckAndSetUIStateForCurrentMap()
{
    UWorld* World = GetWorld();
    if (!World) return;

    FString MapName = World->GetMapName();

    UE_LOG(LogTemp, Warning, TEXT("[UIManager] Detected Map: %s"), *MapName);

    EGameUIState TargetState = EGameUIState::MainMenu;  // 기본값

    // 에디터에서 설정한 TMap 순회
    for (const TPair<FString, EGameUIState>& Pair : MapKeywordToUIState)
    {
        if (MapName.Contains(Pair.Key, ESearchCase::IgnoreCase))
        {
            TargetState = Pair.Value;
            UE_LOG(LogTemp, Log, TEXT("[UIManager] Matched Keyword: %s → UI State: %s"),
                *Pair.Key, *UEnum::GetValueAsString(TargetState));
            break;  // 첫 번째 매칭된 것만 사용
        }
    }
    //UI상태 적용
    SetUIState(TargetState);
}