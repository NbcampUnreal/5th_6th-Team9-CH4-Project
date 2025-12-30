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
    if (!UIWidgetMap.Contains(NewState))// 해당 부분 문제
        return;
    if(!UIWidgetMap[NewState])
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
        //World->ServerTravel(URL);
        //UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/KJH/Test/Lobby")));
        ClearActiveWidgets();
        SetUIState(EGameUIState::Lobby);
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
        //GetWorld()->GetFirstPlayerController()->ClientTravel(URL, ETravelType::TRAVEL_Absolute);

        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            //PC->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
        }
        ClearActiveWidgets();
        SetUIState(EGameUIState::Lobby);
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


}

void UUIManagerSubsystem::ShowWidgetByPath(const FString& WidgetPath)
{
    // 기존 위젯 지우기
    ClearActiveWidgets();

    // 주소로 위젯 클래스 로드
    TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, *WidgetPath);

    if (WidgetClass)
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            // 위젯 생성 및 출력
            ActiveStateWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
            if (ActiveStateWidget)
            {
                ActiveStateWidget->AddToViewport();
                // 입력 모드 설정 (InGame 상태의 설정 적용)
                UpdateInputMode(EGameUIState::InGame);
            }
        }
    }
}
void UUIManagerSubsystem::UpdateUIForCurrentMap()
{
    CheckAndSetUIStateForCurrentMap();  // 기존 맵 이름 확인 로직 재사용
}

void UUIManagerSubsystem::Test() {
        UE_LOG(LogTemp, Warning, TEXT("[UIManager] Initializing UIManagerSubsystem..."));
    // 1. UI 위젯 클래스 등록 (필요한 위젯들을 여기서 LoadClass로 등록)
    // 예시: 실제 경로는 프로젝트에 맞게 수정하세요.
    RegisterUIWidget(EGameUIState::MainMenu, LoadClass<UUserWidget>(nullptr, TEXT("/Game/KJH/WBP_MainTitleWidget.WBP_MainTitleWidget_C")));
    RegisterUIWidget(EGameUIState::Lobby, LoadClass<UUserWidget>(nullptr, TEXT("/Game/KJH/WBP_LobbyWidget.WBP_LobbyWidget_C")));
    RegisterUIWidget(EGameUIState::InGame, LoadClass<UUserWidget>(nullptr, TEXT("/Game/KJH/WBP_GameHUDWidget.WBP_GameHUDWidget_C")));
    RegisterUIWidget(EGameUIState::GameOver, LoadClass<UUserWidget>(nullptr, TEXT("/Game/KJH/WBP_ResultWidget.WBP_ResultWidget_C")));

    // 추가 위젯이 있다면 여기 계속 추가
    // RegisterUIWidget(EGameUIState::Inventory, LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/WBP_Inventory.WBP_Inventory_C")));

    UE_LOG(LogTemp, Warning, TEXT("[UIManager] All UI Widgets Registered!"));

    // 2. 맵 키워드 → UI 상태 매핑 (코드에서 강제 정의 → 에디터 설정 의존성 제거)
    MapKeywordToUIState.Empty();

    MapKeywordToUIState.Add(TEXT("MainTitle"), EGameUIState::MainMenu);
    MapKeywordToUIState.Add(TEXT("Lobby"), EGameUIState::Lobby);
    MapKeywordToUIState.Add(TEXT("Tile"), EGameUIState::InGame);
    MapKeywordToUIState.Add(TEXT("RapidInputRacingMinigame"), EGameUIState::InGame);
    MapKeywordToUIState.Add(TEXT("TimingGame"), EGameUIState::InGame);
    MapKeywordToUIState.Add(TEXT("Result"), EGameUIState::GameOver);

    // 필요 시 추가 (예: 테스트용)
    // MapKeywordToUIState.Add(TEXT("TestMap"), EGameUIState::InGame);

    UE_LOG(LogTemp, Warning, TEXT("[UIManager] Map Keywords Registered! (%d entries)"), MapKeywordToUIState.Num());

    // 3. 맵 변경 감지 타이머 시작
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            MapCheckTimerHandle,
            this,
            &UUIManagerSubsystem::CheckAndSetUIStateForCurrentMap,
            0.5f,
            false  // 한 번만 실행 (맵 로드 직후 충분)
        );

        UE_LOG(LogTemp, Warning, TEXT("[UIManager] MapCheckTimer Started!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UIManager] GetWorld() failed in Initialize!"));
    }

    UE_LOG(LogTemp, Warning, TEXT("[UIManager] Initialization Complete!"));
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
    
    MapName.RemoveFromStart(World->StreamingLevelsPrefix);

    UE_LOG(LogTemp, Warning, TEXT("[UIManager] Detected Map: %s"), *MapName);

    EGameUIState TargetState = EGameUIState::MainMenu;  // 기본값

    // 에디터에서 설정한 TMap 순회
    for (const TPair<FString, EGameUIState>& Pair : MapKeywordToUIState)
    {
        UE_LOG(LogTemp, Warning, TEXT("[UIManager] MAPNAME: %s"), *MapName);
        UE_LOG(LogTemp, Warning, TEXT("[UIManager] Pair.Key: %s"), *Pair.Key);
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