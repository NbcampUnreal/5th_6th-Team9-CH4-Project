#include "Ui/GameHUDWidget.h"

#include "UIManagerSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "MinimapCameraActor.h"
#include "Player/CameraPawn.h"
#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"
#include "Player/MyPlayerController.h"
#include "GameMode/MainGameMode.h"
#include "Inventory/InventoryComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

// 초기화
void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 바인딩
    if (Btn_Dice)       Btn_Dice->OnClicked.AddDynamic(this, &UGameHUDWidget::OnDiceClicked);
    if (Btn_ItemUse)    Btn_ItemUse->OnClicked.AddDynamic(this, &UGameHUDWidget::OnItemUseClicked);
    if (Btn_Inventory)  Btn_Inventory->OnClicked.AddDynamic(this, &UGameHUDWidget::OnInventoryClicked);

    // 캐싱
    if (APlayerController* PC = GetOwningPlayer())
    {
        MyPlayerState = PC->GetPlayerState<AMyPlayerState>();

        if (ACameraPawn* CameraPawn = Cast<ACameraPawn>(PC->GetPawn()))
        {
            InventoryComponent = CameraPawn->GetInventoryComponent();
        }

        // 턴 순서 델리게이트 바인딩 (중요!)
        if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
        {
            AMainGameMode* GM = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
        }
    }

    // 미니맵 배경 연결
    if (Img_MinimapBackground)
    {
        if (UMaterialInstanceDynamic* DynMat = Img_MinimapBackground->GetDynamicMaterial())
        {
            TArray<AActor*> Found;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCameraActor::StaticClass(), Found);
            if (Found.Num() > 0 && Cast<AMinimapCameraActor>(Found[0]))
            {
                AMinimapCameraActor* Cam = Cast<AMinimapCameraActor>(Found[0]);
                if (Cam->MinimapRT)
                {
                    DynMat->SetTextureParameterValue(FName("MapTexture"), Cast<UTexture>(Cam->MinimapRT));
                }
            }
        }
    }

    UpdateHPFromPlayerState();
    UpdateTurnUI();
    UpdateTurnOrderDisplay();
}

// Tick
void UGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdatePlayerMarkers();
    UpdateHPFromPlayerState();
    UpdateTurnUI();
    UpdateTurnOrderDisplay(); // 현재 턴 강조 실시간 갱신
}

// 델리게이트 핸들러
void UGameHUDWidget::OnReceivedFirstOrder(const TArray<int32>& PlayerNumbers, const TArray<int32>& DiceNums)
{
    CurrentTurnOrder = PlayerNumbers; // 서버가 결정한 순서 저장
    UpdateTurnOrderDisplay();

    // 여기서 주사위 굴림 애니메이션 재생 (블루프린트 이벤트 호출 등)
}

void UGameHUDWidget::OnReceivedTurnEndInfo(const TArray<int32>& PlayerNumbers, const TArray<int32>& Scores)
{
    // 순서는 그대로 유지, 필요시 점수 표시 추가
    UpdateTurnOrderDisplay();
}

// 턴 순서 표시
void UGameHUDWidget::UpdateTurnOrderDisplay()
{
    TArray<UTextBlock*> OrderTexts = { Text_TurnOrder1, Text_TurnOrder2, Text_TurnOrder3, Text_TurnOrder4 };

    AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>();
    int32 CurrentTurnPlayer = GM ? GM->GetTurnPlayerNumber() : 0;

    for (int32 i = 0; i < 4; ++i)
    {
        UTextBlock* TextBlock = OrderTexts[i];
        if (!TextBlock) continue;

        if (CurrentTurnOrder.IsValidIndex(i))
        {
            int32 PlayerNum = CurrentTurnOrder[i];
            TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Num%d"), PlayerNum)));

            // 현재 턴인 플레이어는 노란색 강조
            if (PlayerNum == CurrentTurnPlayer)
            {
                TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
                if (Text_CurrentTurn)
                {
                    Text_CurrentTurn->SetText(FText::FromString(FString::Printf(TEXT("Now Turn: Num %d Player"), PlayerNum)));
                }
            }
            else
            {
                TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
            }
        }
        else
        {
            TextBlock->SetText(FText::FromString(TEXT("-")));
            TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
        }
    }
}

void UGameHUDWidget::UpdateHPFromPlayerState()
{
    if (MyPlayerState) UpdateHP(MyPlayerState->GetHP(), MyPlayerState->GetMaxHP());
}

void UGameHUDWidget::UpdateHP(float CurrentHp, float MaxHp)
{
    if (HP_Bar && MaxHp > 0.0f) HP_Bar->SetPercent(CurrentHp / MaxHp);

    if (Text_PlayerHP)
    {
        Text_PlayerHP->SetText(FText::Format(FText::FromString(TEXT("{0} / {1}")),
            FMath::FloorToInt(CurrentHp), FMath::FloorToInt(MaxHp)));
    }
}

void UGameHUDWidget::UpdateTurnUI()
{
    if (!MyPlayerState) return;

    AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>();
    if (!GM) return;

    bool bIsMyTurn = (GM->GetTurnPlayerNumber() == MyPlayerState->PlayerNumber);

    if (Btn_Dice)       Btn_Dice->SetIsEnabled(bIsMyTurn);
    if (Btn_Inventory)  Btn_Inventory->SetIsEnabled(bIsMyTurn);
}

void UGameHUDWidget::UpdatePlayerMarkers()
{
    TArray<UImage*> MarkerWidgets = { Img_PlayerMarker_0, Img_PlayerMarker_1, Img_PlayerMarker_2, Img_PlayerMarker_3 };

    for (UImage* Marker : MarkerWidgets)
    {
        if (Marker) Marker->SetVisibility(ESlateVisibility::Hidden);
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC || !PC->GetPawn()) continue;

        ACameraPawn* CameraPawn = Cast<ACameraPawn>(PC->GetPawn());
        if (!CameraPawn) continue;

        APlayerCharacter* PlayerChar = CameraPawn->GetPlayerCharacter();
        if (!IsValid(PlayerChar)) continue;

        AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>();
        if (!PS) continue;

        int32 Idx = PS->PlayerNumber - 1;
        if (!MarkerWidgets.IsValidIndex(Idx)) continue;

        UImage* Marker = MarkerWidgets[Idx];
        if (!Marker) continue;

        FVector Loc = PlayerChar->GetActorLocation();

        float AlphaX = FMath::GetMappedRangeValueClamped(FVector2D(WorldMapMinX, WorldMapMaxX), FVector2D(0.f, 1.f), Loc.X);
        float AlphaY = FMath::GetMappedRangeValueClamped(FVector2D(WorldMapMinY, WorldMapMaxY), FVector2D(0.f, 1.f), Loc.Y);

        FVector2D MinimapPos(AlphaX * MinimapSize, AlphaY * MinimapSize);

        Marker->SetRenderTranslation(MinimapPos);
        Marker->SetVisibility(ESlateVisibility::Visible);
    }
}

void UGameHUDWidget::OnDiceClicked()
{
    if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetOwningPlayer()))
    {
        MyPC->Server_RequestThrowDice();
    }
}

void UGameHUDWidget::OnItemUseClicked()
{
    if (ACameraPawn* CameraPawn = Cast<ACameraPawn>(GetOwningPlayerPawn()))
    {
        if (UInventoryComponent* Inv = CameraPawn->GetInventoryComponent())
        {
            for (int32 i = 0; i < UInventoryComponent::Max_SLOTS; ++i)
            {
                if (!Inv->IsSlotEmpty(i))
                {
                    Inv->UseItem(i);
                    return;
                }
            }
        }
    }
}

void UGameHUDWidget::OnInventoryClicked()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        if (bIsInventoryOpen)
        {
            UISubsystem->SetUIState(EGameUIState::InGame);  // 인벤토리 닫기
        }
        else
        {
            UISubsystem->SetUIState(EGameUIState::Inventory);  // 인벤토리 열기 (겹치게끔)
        }
        bIsInventoryOpen = !bIsInventoryOpen;
    }
}