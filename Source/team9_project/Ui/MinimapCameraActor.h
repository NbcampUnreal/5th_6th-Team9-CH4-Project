#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"  // 이 include 필수!
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
    // 캡처 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
    USceneCaptureComponent2D* CaptureComp;

    // 렌더 타겟
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    UTextureRenderTarget2D* MinimapRT;

    //OrthoWidth 등 에디터에서 조정 가능
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    float OrthoWidth = 12000.0f;
};