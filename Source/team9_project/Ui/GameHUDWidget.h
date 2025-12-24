#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapCameraActor.h"
#include "GameHUDWidget.generated.h"

class UButton;
class UImage;
class UProgressBar;
class UTextBlock;
class UMaterialInstanceDynamic;

UCLASS()
class TEAM9_PROJECT_API UGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    AMinimapCameraActor* FindMinimapCamera();

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 버튼 클릭 핸들러
    void OnDiceClicked();

    void OnItemUseClicked();

public:
    // HP 갱신 함수 (외부에서 호출 가능)
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHP(float CurrentHp, float MaxHp);

    // 미니맵 마커 갱신
    void UpdatePlayerMarkers();

protected:
    // --- 버튼 바인딩 ---
    UPROPERTY(meta = (BindWidget))
    UButton* Btn_Dice;

    UPROPERTY(meta = (BindWidget))
    UButton* Btn_ItemUse;

    // --- HP 바인딩 ---
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HP_Bar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_PlayerHP;

    // --- 미니맵 배경 ---
    UPROPERTY(meta = (BindWidget))
    UImage* Img_MinimapBackground;

    // --- 플레이어 마커 (4명) ---
    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_0;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_1;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_2;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_3;

private:
    // 미니맵 월드 범위 (맵 크기에 맞게 조정)
    float WorldMapMinX = -5000.0f;
    float WorldMapMaxX = 5000.0f;
    float WorldMapMinY = -5000.0f;
    float WorldMapMaxY = 5000.0f;

    // 미니맵 UMG 크기 (정사각형 기준)
    float MinimapSize = 200.0f;
};