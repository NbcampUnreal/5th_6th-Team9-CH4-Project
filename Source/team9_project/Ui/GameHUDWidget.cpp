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

// �ʱ�ȭ
void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. ��ư ���ε� (������ ����)
    if (Btn_Dice)
    {
        Btn_Dice->OnClicked.AddDynamic(this, &UGameHUDWidget::OnDiceClicked);
    } 

    // 2. OwningPlayer �����ϰ� ��������
    APlayerController* PC = GetOwningPlayer();
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameHUD] OwningPlayer is null in NativeConstruct"));
        return;  // �� �̻� ���� �� ��
    }

    // 3. PlayerState ĳ��
    MyPlayerState = PC->GetPlayerState<AMyPlayerState>();

    // 4. Pawn ĳ���� �� �κ��丮 ������Ʈ
    if (ACameraPawn* CameraPawn = Cast<ACameraPawn>(PC->GetPawn()))
    {
        InventoryComponent = CameraPawn->GetInventoryComponent();
    }

    // 5. ��������Ʈ ���ε� (�ߺ� ���� ����)
    if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
    {
        if (!bDelegatesBound)
        {
            MyPC->OnFirstReady.AddDynamic(this, &UGameHUDWidget::OnReceivedFirstOrder);
            MyPC->TurnEndInfo.AddDynamic(this, &UGameHUDWidget::OnReceivedTurnEndInfo);
            bDelegatesBound = true;
            UE_LOG(LogTemp, Log, TEXT("[GameHUD] Delegates bound successfully"));
        }
    }

    // 6. �̴ϸ� ��� ���� (Ŭ���̾�Ʈ������, �ߺ� ����)
    //if (Img_MinimapBackground && PC->IsLocalController())  // ���� Ŭ���̾�Ʈ������
    //{
    //    if (UMaterialInstanceDynamic* DynMat = Img_MinimapBackground->GetDynamicMaterial())
    //    {
    //        TArray<AActor*> Found;
    //        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCameraActor::StaticClass(), Found);
    //        if (Found.Num() > 0)
    //        {
    //            if (AMinimapCameraActor* Cam = Cast<AMinimapCameraActor>(Found[0]))
    //            {
    //                if (Cam->MinimapRT)
    //                {
    //                    DynMat->SetTextureParameterValue(FName("MapTexture"), Cast<UTexture>(Cam->MinimapRT));
    //                }
    //            }
    //        }
    //    }
    //}

    // �ʱ� UI ������Ʈ
    UpdateHPFromPlayerState();
    UpdateTurnUI();
    UpdateTurnOrderDisplay();
}

void UGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!MyPlayerState)
    {
        UE_LOG(LogTemp, Log, TEXT("Non MyPlayer State"));
        return;
    }
    UpdatePlayerMarkers();
    UpdateHPFromPlayerState();
    UpdateTurnUI();
    UpdateTurnOrderDisplay(); // ���� �� ���� �ǽð� ����
}

void UGameHUDWidget::OnDiceResultReceived(int32 PlayerNumber, int32 DiceNum)
{
    //OnDiceResultReceived.Broadcast(PlayerNumber, DiceNum);
    UE_LOG(LogTemp, Log, TEXT("Dice Result: Player %d rolled %d"), PlayerNumber, DiceNum);
}

// ��������Ʈ �ڵ鷯
void UGameHUDWidget::OnReceivedFirstOrder(TArray<int32> PlayerNumbers, TArray<int32> DiceNums)
{
    CurrentTurnOrder = PlayerNumbers; // ������ ������ ���� ����
    //UE_LOG(LogTemp, Log, TEXT("PlayerNUM %d"), CurrentTurnOrder);
    UpdateTurnOrderDisplay();

    // TODO: �ֻ��� ���� �ִϸ��̼� ��� (�������Ʈ �̺�Ʈ ȣ�� ��)
}

void UGameHUDWidget::OnReceivedTurnEndInfo(TArray<int32> PlayerNumbers, TArray<int32> Scores, EEndType EndType)
{
    // ������ �״�� ����, �ʿ�� ���� ǥ�� �߰�
    UpdateTurnOrderDisplay();
}

// �� ���� ǥ��
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

            // ���� ���� �÷��̾�� ����� ����
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
    //if (Btn_Inventory)  Btn_Inventory->SetIsEnabled(bIsMyTurn);
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
    // 주사위
    /*if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetOwningPlayer()))
    {
        MyPC->Server_RequestThrowDice();
    }*/

    AMyPlayerController* PC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (IsValid(PC) == false)
    {
        UE_LOG(LogTemp, Warning, TEXT("PC Not Casting"));
    } 
    ACameraPawn* Pawn = Cast<ACameraPawn>(PC->GetPawn());
    if (IsValid(Pawn) == false)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pawn Not Casting"));
    }

    if (Pawn->GetInventoryComponent() && Pawn->GetInventoryComponent()->IsUsingItem())
    {
        Pawn->ServerRPC_ConfirmItemUse();
        return;
    }

    Pawn->ServerRPCMove();
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
            UISubsystem->SetUIState(EGameUIState::InGame);  // �κ��丮 �ݱ�
        }
        else
        {
            UISubsystem->SetUIState(EGameUIState::Inventory);  // �κ��丮 ���� (��ġ�Բ�)
        }
        bIsInventoryOpen = !bIsInventoryOpen;
    }
}