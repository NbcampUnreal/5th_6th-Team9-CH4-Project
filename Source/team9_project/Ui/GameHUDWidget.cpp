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
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"

void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 바인딩
    if (Btn_Dice)
    {
        Btn_Dice->OnClicked.AddDynamic(this, &UGameHUDWidget::OnDiceClicked);
    }
    if (Btn_ItemUse)
    {
        Btn_ItemUse->OnClicked.AddDynamic(this, &UGameHUDWidget::OnItemUseClicked);
    }
    if (Btn_Inventory)
    {
        Btn_Inventory->OnClicked.AddDynamic(this, &UGameHUDWidget::OnInventoryClicked);
    }
    // PlayerState 및 Inventory 캐싱
    if (APlayerController* PC = GetOwningPlayer())
    {
        MyPlayerState = PC->GetPlayerState<AMyPlayerState>();

        if (ACameraPawn* CameraPawn = Cast<ACameraPawn>(PC->GetPawn()))
        {
            InventoryComponent = CameraPawn->GetInventoryComponent();
        }
    }

    // 미니맵 배경 텍스처 연결
    if (Img_MinimapBackground)
    {
        UMaterialInstanceDynamic* DynMat = Img_MinimapBackground->GetDynamicMaterial();
        if (DynMat)
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCameraActor::StaticClass(), FoundActors);
            if (FoundActors.Num() > 0)
            {
                if (AMinimapCameraActor* Cam = Cast<AMinimapCameraActor>(FoundActors[0]))
                {
                    if (Cam->MinimapRT)
                    {
                        DynMat->SetTextureParameterValue(FName("MapTexture"), Cast<UTexture>(Cam->MinimapRT));
                    }
                }
            }
        }
    }

    // 초기 HP 표시
    UpdateHPFromPlayerState();
    UpdateTurnUI();
}

void UGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdatePlayerMarkers();
    UpdateHPFromPlayerState();
    UpdateTurnUI();
}

void UGameHUDWidget::UpdateHPFromPlayerState()
{
    if (MyPlayerState)
    {
        UpdateHP(MyPlayerState->GetHP(), MyPlayerState->GetMaxHP());
    }
}

void UGameHUDWidget::UpdateTurnUI()
{
    if (!MyPlayerState) return;

    AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>();
    if (!GM) return;

    bool bIsMyTurn = (GM->GetTurnPlayerNumber() == MyPlayerState->PlayerNumber);

    // 주사위 버튼: 내 턴만 되면 무조건 활성화
    if (Btn_Dice)
    {
        Btn_Dice->SetIsEnabled(bIsMyTurn);
    }

    //  아이템 사용 버튼: 내 턴 + 이번 턴 아이템 미사용 시만
    //if (Btn_ItemUse && InventoryComponent)
    //{
    //    Btn_ItemUse->SetIsEnabled(bIsMyTurn && InventoryComponent->CanUseItem());
    //}

    // 인벤토리 열기 버튼은 언제나 가능 (보기만 하니까)
    if (Btn_Inventory)
    {
        Btn_Inventory->SetIsEnabled(bIsMyTurn);  // 턴 중에만 열기 가능하게 (선택사항)
    }
}

AMinimapCameraActor* UGameHUDWidget::FindMinimapCamera()
{
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCameraActor::StaticClass(), Found);
    return Found.Num() > 0 ? Cast<AMinimapCameraActor>(Found[0]) : nullptr;
}

void UGameHUDWidget::OnDiceClicked()
{
    if (!MyPlayerState) return;

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
        {
            MyPC->Server_RequestThrowDice();

            UE_LOG(LogTemp, Warning, TEXT("Dice Button Clicked - Player %d requested dice roll"), MyPlayerState->PlayerNumber);
        }
    }
}

void UGameHUDWidget::OnItemUseClicked()
{
    if (ACameraPawn* CameraPawn = Cast<ACameraPawn>(GetOwningPlayerPawn()))
    {
        if (UInventoryComponent* Inv = CameraPawn->GetInventoryComponent())
        {
            // 첫 번째 사용 가능한 아이템 슬롯 찾기
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
    if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        if (bIsInventoryOpen)
        {
            if (Btn_ItemUse) Btn_ItemUse->SetIsEnabled(false);  // 창 열렸을 때 빠른 사용 막기
        }
        else
        {
            UpdateTurnUI();  // 창 닫히면 원래 상태 복구
        }
    }
}

void UGameHUDWidget::UpdateHP(float CurrentHp, float MaxHp)
{
    if (HP_Bar && MaxHp > 0.0f)
    {
        HP_Bar->SetPercent(CurrentHp / MaxHp);
    }

    if (Text_PlayerHP)
    {
        FText HPText = FText::Format(FText::FromString(TEXT("{0} / {1}")),
            FMath::FloorToInt(CurrentHp),
            FMath::FloorToInt(MaxHp));
        Text_PlayerHP->SetText(HPText);
    }
}

void UGameHUDWidget::UpdatePlayerMarkers()
{
    TArray<UImage*> MarkerWidgets = {
        Img_PlayerMarker_0,
        Img_PlayerMarker_1,
        Img_PlayerMarker_2,
        Img_PlayerMarker_3
    };

    // 모든 마커 숨김
    for (UImage* Marker : MarkerWidgets)
    {
        if (Marker)
        {
            Marker->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 모든 플레이어 순회
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

        float AlphaX = FMath::GetMappedRangeValueClamped(
            FVector2D(WorldMapMinX, WorldMapMaxX),
            FVector2D(0.f, 1.f),
            Loc.X
        );
        float AlphaY = FMath::GetMappedRangeValueClamped(
            FVector2D(WorldMapMinY, WorldMapMaxY),
            FVector2D(0.f, 1.f),
            Loc.Y
        );

        FVector2D MinimapPos(AlphaX * MinimapSize, AlphaY * MinimapSize);

        Marker->SetRenderTranslation(MinimapPos);
        Marker->SetVisibility(ESlateVisibility::Visible);
    }
}