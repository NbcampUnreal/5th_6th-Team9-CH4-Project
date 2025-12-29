#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUDWidget.generated.h"

class UUIManagerSubsystem;
class APlayerController;
class AMyPlayerState;
class AMainGameMode;
class ACameraPawn;
class APlayerCharacter;
class UInventoryComponent;
class AMinimapCameraActor;

class UButton;
class UProgressBar;
class UImage;
class UTextBlock;

UCLASS()
class TEAM9_PROJECT_API UGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 버튼 핸들러
    void OnDiceClicked();
    void OnItemUseClicked();
    void OnInventoryClicked();
    bool bIsInventoryOpen = false;

    // UI 업데이트 함수들
    void UpdateHP(float CurrentHp, float MaxHp);
    void UpdateHPFromPlayerState();
    void UpdateTurnUI();

    // 미니맵 관련
    AMinimapCameraActor* FindMinimapCamera();
    void UpdatePlayerMarkers();

private:
    // 캐싱된 객체들
    AMyPlayerState* MyPlayerState = nullptr;
    UInventoryComponent* InventoryComponent = nullptr;

    // 미니맵 월드 범위 (에디터에서 조정 가능하게 UPROPERTY로 변경 추천)
    float WorldMapMinX = -5000.0f;
    float WorldMapMaxX = 5000.0f;
    float WorldMapMinY = -5000.0f;
    float WorldMapMaxY = 5000.0f;
    float MinimapSize = 200.0f;  // 미니맵 UMG 위젯 크기 (정사각형 기준)

protected:
    // --- 버튼 바인딩 ---
    UPROPERTY(meta = (BindWidget))
    UButton* Btn_Dice;

    UPROPERTY(meta = (BindWidget))
    UButton* Btn_ItemUse;

    UPROPERTY(meta = (BindWidget))
    UButton* Btn_Inventory;

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
};