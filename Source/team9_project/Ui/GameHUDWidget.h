#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/MainGameMode.h"
#include "GameHUDWidget.generated.h"

class AMyPlayerState;
class AMainGameMode;
class ACameraPawn;
class APlayerCharacter;
class UInventoryComponent;
class AMinimapCameraActor;
class AMyPlayerController;

class UButton;
class UProgressBar;
class UImage;
class UTextBlock;

UCLASS()
class TEAM9_PROJECT_API UGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    // 델리게이트 핸들러 (턴 순서 및 라운드 종료 정보 수신)
    UFUNCTION()
    void OnDiceResultReceived(int32 PlayerNumber, int32 DiceNum);

    UFUNCTION()
    void OnReceivedFirstOrder(TArray<int32> PlayerNumbers, TArray<int32> DiceNums);

    UFUNCTION()
    void OnReceivedTurnEndInfo(TArray<int32> PlayerNumbers, TArray<int32> Scores, EEndType EndType);
private:
    // 버튼 핸들러
    void OnDiceClicked();
    void OnItemUseClicked();
    void OnInventoryClicked();

    // UI 업데이트
    void UpdateHP(float CurrentHp, float MaxHp);
    void UpdateHPFromPlayerState();
    void UpdateTurnUI();
    void UpdatePlayerMarkers();
    void UpdateTurnOrderDisplay(); // 턴 순서 표시 업데이트


    AMinimapCameraActor* FindMinimapCamera();

private:
    //중복 바인딩 방지
    bool bDelegatesBound = false;
    // 캐싱된 객체
    AMyPlayerState* MyPlayerState = nullptr;
    UInventoryComponent* InventoryComponent = nullptr;

    // 턴 순서 저장 (서버에서 받은 그대로)
    TArray<int32> CurrentTurnOrder;

    // 미니맵 변환용 상수
    float WorldMapMinX = -5000.0f;
    float WorldMapMaxX = 5000.0f;
    float WorldMapMinY = -5000.0f;
    float WorldMapMaxY = 5000.0f;
    float MinimapSize = 200.0f;

    bool bIsInventoryOpen = false;

protected:
    // 기존 BindWidget
    UPROPERTY(meta = (BindWidget))
    UButton* Btn_Dice;

    //UPROPERTY(meta = (BindWidget))
    //UButton* Btn_ItemUse;

    //UPROPERTY(meta = (BindWidget))
    //UButton* Btn_Inventory;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HP_Bar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_PlayerHP;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_MinimapBackground;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_0;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_1;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_2;

    UPROPERTY(meta = (BindWidget))
    UImage* Img_PlayerMarker_3;

    // 턴 순서 표시용
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder1;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder2;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder3;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder4;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_CurrentTurn; // "현재 턴: X번 플레이어"
};