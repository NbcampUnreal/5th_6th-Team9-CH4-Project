#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MinimapWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UMinimapWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 머티리얼 인스턴스를 동적으로 제어하기 위한 함수
    void SetMinimapZoom(float ZoomValue);

    UPROPERTY(meta = (BindWidget))
    UImage* Minimap_Display;
};