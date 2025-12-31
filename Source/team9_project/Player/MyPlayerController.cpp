#include "Player/MyPlayerController.h"
#include "Ui/UIManagerSubsystem.h"
#include "Ui/MinimapCameraActor.h"
#include "GameMode/MainGameMode.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Player/MyPlayerState.h"
#include "Ui/ResultWidget.h"

AMyPlayerController::AMyPlayerController()
{
    bReplicates = true;
    bShowMouseCursor = true;
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    UE_LOG(LogTemp, Warning, TEXT("Possessed: %s"), *GetNameSafe(InPawn));
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 컨트롤러(클라이언트)에서만 실행
    if (!IsLocalController())
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("APlayerController::BeginPlay"));

    // 1. Enhanced Input 설정
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem
        = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    EnhancedInputLocalPlayerSubsystem->AddMappingContext(BoardIMC, 0);
    CurrentIMC = BoardIMC;

    // 2. 미니맵 카메라 스폰 (이미 있으면 스폰 안 함)
    UWorld* World = GetWorld();
    if (!World) return;

    if (!MinimapCamera)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        MinimapCamera = World->SpawnActor<AMinimapCameraActor>(
            AMinimapCameraActor::StaticClass(), 
            FVector(0, 0, 10000), 
            FRotator(-90, 0, 0), 
            SpawnParams);

        if (MinimapCamera)
        {
            MinimapCamera->CaptureComp->OrthoWidth = 12000.0f;
            if (MinimapCamera->MinimapRT)
            {
                MinimapCamera->CaptureComp->TextureTarget = MinimapCamera->MinimapRT;
            }
        }
    }

    // 3. === 핵심: UIManager에 현재 맵 UI 업데이트 요청 ===
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UUIManagerSubsystem* Subsystem = GI->GetSubsystem<UUIManagerSubsystem>())
        {
            Subsystem->UpdateUIForCurrentMap();  // 맵 이름 보고 적절한 UI 띄움
            UE_LOG(LogTemp, Log, TEXT("[PlayerController] UI Update requested to UIManager"));
        }
    }

    // 4. 인벤토리 위젯 생성은 주석 처리 (GameHUDWidget 버튼으로 토글)
    if (InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport();  // 중복 생성 문제 해결됨
        }
    }
}

void AMyPlayerController::Server_RequestThrowDice_Implementation()
{
    if (AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>())
    {
        if (AMyPlayerState* Ps = GetPlayerState<AMyPlayerState>())
        {
            int32 Pn = Ps->GetPlayerNumber();
            GM->ThrowDice(Pn);
        }
    }
}

void AMyPlayerController::Client_SetUIState_Implementation(EGameUIState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("Client_SetUIState 실행 상태: %s"), *UEnum::GetValueAsString(NewState));
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->SetUIState(NewState);
    }
}

void AMyPlayerController::Client_ReceiveDiceResult_Implementation(int32 PlayerNumber, int32 DiceNum)
{
    OnDiceResultReceived.Broadcast(PlayerNumber, DiceNum);
}

void AMyPlayerController::Multicast_ShowResult_Implementation()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->SetUIState(EGameUIState::GameOver);

        if (UResultWidget* ResultUI = UISubsystem->GetActiveWidget<UResultWidget>())
        {
            FTimerHandle ResultDelayHandle;
            GetWorld()->GetTimerManager().SetTimer(ResultDelayHandle, [ResultUI]()
            {
                if (ResultUI)
                {
                    ResultUI->SetupResults();
                }
            }, 0.5f, false);
        }
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AMyPlayerController::TestShowResult);
    }
}

void AMyPlayerController::Client_ReceiveFirstOrder_Implementation(const TArray<int32>& PlayerNumbers, const TArray<int32>& DiceNums)
{
    OnFirstReady.Broadcast(PlayerNumbers, DiceNums);
}

void AMyPlayerController::Client_ReceiveTurnEndInfo_Implementation(const TArray<int32>& PlayerNumbers, const TArray<int32>& Scores, EEndType EndType)
{
	TurnEndInfo.Broadcast(PlayerNumbers, Scores, EndType);
}

void AMyPlayerController::TestShowResult()
{
    UE_LOG(LogTemp, Warning, TEXT("[Test] R Key Press - Result Widget Coll"));
    if (HasAuthority())
    {
        if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
        {
            UISubsystem->SetUIState(EGameUIState::Result);
        }
    }
}