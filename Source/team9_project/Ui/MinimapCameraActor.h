#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MinimapCameraActor.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMinimapCameraActor : public AActor
{
    GENERATED_BODY()

public:
    AMinimapCameraActor();

protected:
    virtual void BeginPlay() override;

public:
    // 캡처를 담당하는 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
    USceneCaptureComponent2D* CaptureComp;

    // 렌더링 결과가 저장될 텍스처 (에디터에서 생성한 RT를 할당)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    class UTextureRenderTarget2D* MinimapRT;
};