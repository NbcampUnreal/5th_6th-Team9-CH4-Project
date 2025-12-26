#include "Ui/MinimapCameraActor.h"
#include "Engine/TextureRenderTarget2D.h"

AMinimapCameraActor::AMinimapCameraActor()
{
    // 0. 서버 복제 방지
    bReplicates = false;
    // 1. 컴포넌트 생성 및 루트 설정
    CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComp"));
    RootComponent = CaptureComp;

    // 2. 기본 방향 설정 (아래를 내려다보도록 Pitch -90)
    CaptureComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    // 3. 캡처 옵션 설정
    CaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
    CaptureComp->OrthoWidth = 5000.0f; // 맵 크기에 맞게 조절
    CaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

    // 매 프레임 업데이트할지 선택 (성능 최적화 시 false 후 필요할 때만 호출)
    CaptureComp->bCaptureEveryFrame = true;
    CaptureComp->bCaptureOnMovement = true;
}

void AMinimapCameraActor::BeginPlay()
{
    Super::BeginPlay();

    // 에디터에서 할당한 렌더 타겟을 컴포넌트에 연결
    if (MinimapRT)
    {
        CaptureComp->TextureTarget = MinimapRT;
    }
}