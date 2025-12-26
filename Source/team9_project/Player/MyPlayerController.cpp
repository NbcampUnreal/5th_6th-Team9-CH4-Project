#include "Player/MyPlayerController.h"
#include "Ui/Test/MyTestGameMode.h"
#include "Ui/UIManagerSubsystem.h"
#include "Ui/MinimapCameraActor.h"
#include "GameMode/MainGameMode.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Player/MyPlayerState.h"
#include "Ui/ResultWidget.h"
#include <Kismet/GameplayStatics.h>


AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;

	bShowMouseCursor = true;
	//bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Possessed: %s"), *GetNameSafe(InPawn));
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
    //Dedicated Server에서는 미니맵 카메라 관련 로직 스킵
    ENetMode NetMode = GetWorld()->GetNetMode();
    if (NetMode == NM_DedicatedServer)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MyPlayerController] Dedicated Server detected - Skipping MinimapCamera spawn and input setup"));
        return;
    }
	// 내 클라에서만 실행?
	if (!IsLocalController())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("APlayerController::BeginPlay"));

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (LocalPlayer)
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        if (Subsystem && BoardIMC)
        {
            Subsystem->AddMappingContext(BoardIMC, 0);
            CurrentIMC = BoardIMC;
        }
    }
    // 미니맵 카메라 스폰 (클라이언트에서만)
    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCameraActor::StaticClass(), FoundCameras);
    if (FoundCameras.Num() > 0)
    {
        MinimapCamera = Cast<AMinimapCameraActor>(FoundCameras[0]);
        UE_LOG(LogTemp, Log, TEXT("[MyPlayerController] Found existing MinimapCamera"));
    }
    else
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        MinimapCamera = GetWorld()->SpawnActor<AMinimapCameraActor>(
            AMinimapCameraActor::StaticClass(),
            FVector(0, 0, 10000),
            FRotator(-90, 0, 0),
            SpawnParams
        );

        if (MinimapCamera)
        {
            MinimapCamera->CaptureComp->OrthoWidth = 12000.0f;

            if (MinimapCamera->MinimapRT)
            {
                MinimapCamera->CaptureComp->TextureTarget = MinimapCamera->MinimapRT;
                UE_LOG(LogTemp, Log, TEXT("[MyPlayerController] MinimapCamera spawned and configured"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[MyPlayerController] MinimapRT is null!"));
            }
        }
    }
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem
		= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(BoardIMC, 0);
	CurrentIMC = BoardIMC;

	//UI_PART
	UWorld* World = GetWorld();
	if (!World) return;

	// 이미 있으면 스폰 안 함
	if (MinimapCamera) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MinimapCamera = World->SpawnActor<AMinimapCameraActor>(AMinimapCameraActor::StaticClass(), FVector(0, 0, 10000), FRotator(-90, 0, 0), SpawnParams);

	if (MinimapCamera)
	{
		MinimapCamera->CaptureComp->OrthoWidth = 12000.0f;  // 맵 크기
		if (MinimapCamera->MinimapRT)
		{
			MinimapCamera->CaptureComp->TextureTarget = MinimapCamera->MinimapRT;
		}
	}
	// Cho_Sungmin - 인벤토리 위젯 생성
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
		}
	}
}
// 모든 RPC는 뒤에 _Implementation을 붙여서 본체를 만듭니다.

void AMyPlayerController::Server_RequestThrowDice_Implementation()
{
    // AMyTestGameMode로 캐스팅하여 주사위 로직 처리 요청
    if (AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>())
    {
        AMyPlayerState* Ps = GetPlayerState<AMyPlayerState>();
        int32 Pn = Ps -> GetPlayerNumber();
        GM->ThrowDice(Pn);
    }
}

void AMyPlayerController::Client_SetUIState_Implementation(EGameUIState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("Client_SetUIState 실행 상태: %s"), *UEnum::GetValueAsString(NewState));
    // UUIManagerSubsystem을 사용하여 중앙 제어식 UI 변경
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->SetUIState(NewState);
    }
}

void AMyPlayerController::Client_ReceiveDiceResult_Implementation(int32 PlayerNumber, int32 DiceNum)
{
    // UI나 블루프린트에서 들을 수 있도록 델리게이트 브로드캐스트
    OnDiceResultReceived.Broadcast(PlayerNumber, DiceNum);
}

void AMyPlayerController::Multicast_ShowResult_Implementation()
{
    // 각 클라이언트(및 서버 자신)의 로컬 환경에서 실행됨
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        // 1. UI 상태를 GameOver(결과창)로 전환
        UISubsystem->SetUIState(EGameUIState::GameOver);

        // 2. 생성된 결과창 위젯을 가져와서 데이터 세팅
        if (UResultWidget* ResultUI = UISubsystem->GetActiveWidget<UResultWidget>())
        {
            // PlayerState의 복제 데이터가 도착할 시간을 고려해 0.2~0.5초 딜레이 후 세팅 권장
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
        // R 키 눌렀을 때 결과 화면 강제 표시 (테스트용)
        InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AMyPlayerController::TestShowResult);
    }
}

void AMyPlayerController::TestShowResult()
{
    UE_LOG(LogTemp, Warning, TEXT("[Test] R Key Press - Result Widget Coll"));
    // 서버에서만 EndMatch 호출 (호스트가 누르면 모든 클라이언트 결과 화면 뜸)
    if (HasAuthority())
    {
        if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
        {
            UISubsystem->SetUIState(EGameUIState::Result);
        }
    }
}