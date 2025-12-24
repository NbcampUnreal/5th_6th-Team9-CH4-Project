#include "UI/MinimapWidget.h"
#include "Materials/MaterialInstanceDynamic.h"

void UMinimapWidget::SetMinimapZoom(float ZoomValue)
{
    if (Minimap_Display)
    {
        // Image 위젯에 사용된 머티리얼을 가져와 파라미터 조절
        UMaterialInstanceDynamic* DynMat = Minimap_Display->GetDynamicMaterial();
        if (DynMat)
        {
            DynMat->SetScalarParameterValue(FName("Zoom"), ZoomValue);
        }
    }
}