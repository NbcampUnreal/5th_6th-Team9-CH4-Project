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
    // ��������Ʈ �ڵ鷯 (�� ���� �� ���� ���� ���� ����)
    UFUNCTION()
    void OnDiceResultReceived(int32 PlayerNumber, int32 DiceNum);

    UFUNCTION()
    void OnReceivedFirstOrder(TArray<int32> PlayerNumbers, TArray<int32> DiceNums);

    UFUNCTION()
    void OnReceivedTurnEndInfo(TArray<int32> PlayerNumbers, TArray<int32> Scores, EEndType EndType);
private:
    // ��ư �ڵ鷯
    UFUNCTION()
    void OnDiceClicked();
    void OnItemUseClicked();
    void OnInventoryClicked();

    // UI ������Ʈ
    void UpdateHP(float CurrentHp, float MaxHp);
    void UpdateHPFromPlayerState();
    void UpdateTurnUI();
    void UpdatePlayerMarkers();
    void UpdateTurnOrderDisplay(); // �� ���� ǥ�� ������Ʈ


    AMinimapCameraActor* FindMinimapCamera();

private:
    //�ߺ� ���ε� ����
    bool bDelegatesBound = false;
    // ĳ�̵� ��ü
    AMyPlayerState* MyPlayerState = nullptr;
    UInventoryComponent* InventoryComponent = nullptr;

    // �� ���� ���� (�������� ���� �״��)
    TArray<int32> CurrentTurnOrder;

    // �̴ϸ� ��ȯ�� ���
    float WorldMapMinX = -5000.0f;
    float WorldMapMaxX = 5000.0f;
    float WorldMapMinY = -5000.0f;
    float WorldMapMaxY = 5000.0f;
    float MinimapSize = 200.0f;

    bool bIsInventoryOpen = false;

protected:
    // ���� BindWidget
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

    // �� ���� ǥ�ÿ�
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder1;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder2;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder3;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TurnOrder4;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_CurrentTurn; // "���� ��: X�� �÷��̾�"
};