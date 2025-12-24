#include "MyTestPlayerController.h"
#include "MyTestGameMode.h"
#include "Ui/UIManagerSubsystem.h"
#include "Ui/MinimapCameraActor.h"
#include "Ui/ResultWidget.h"

void AMyTestPlayerController::BeginPlay()
{
    Super::BeginPlay();

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
}

// 모든 RPC는 뒤에 _Implementation을 붙여서 본체를 만듭니다.

void AMyTestPlayerController::Server_RequestThrowDice_Implementation()
{
    // AMyTestGameMode로 캐스팅하여 주사위 로직 처리 요청
    if (AMyTestGameMode* GM = GetWorld()->GetAuthGameMode<AMyTestGameMode>())
    {
        GM->ProcessDiceThrow(this);
    }
}

void AMyTestPlayerController::Client_SetUIState_Implementation(EGameUIState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("Client_SetUIState 실행 상태: %s"), *UEnum::GetValueAsString(NewState));
    // UUIManagerSubsystem을 사용하여 중앙 제어식 UI 변경
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->SetUIState(NewState);
    }
}

void AMyTestPlayerController::Client_ReceiveDiceResult_Implementation(int32 Result)
{
    // UI나 블루프린트에서 들을 수 있도록 델리게이트 브로드캐스트
    OnDiceResultReceived.Broadcast(Result);
}

void AMyTestPlayerController::Multicast_ShowResult_Implementation()
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

void AMyTestPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        // R 키 눌렀을 때 결과 화면 강제 표시 (테스트용)
        InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AMyTestPlayerController::TestShowResult);
    }
}

void AMyTestPlayerController::TestShowResult()
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