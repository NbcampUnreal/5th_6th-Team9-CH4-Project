#include "Ui/GameHUDWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "MinimapCameraActor.h"
#include "Player/MyPlayerState.h"
#include "Engine/TextureRenderTarget2D.h"

void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 바인딩 (기존 코드)
    if (Btn_Dice) Btn_Dice->OnClicked.AddDynamic(this, &UGameHUDWidget::OnDiceClicked);
    if (Btn_ItemUse) Btn_ItemUse->OnClicked.AddDynamic(this, &UGameHUDWidget::OnItemUseClicked);

    // 미니맵 배경 렌더타겟 연결
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

    UpdateHP(100.f, 100.f);
}
// 카메라 찾기 헬퍼 함수
AMinimapCameraActor* UGameHUDWidget::FindMinimapCamera()
{
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCameraActor::StaticClass(), Found);
    return Found.Num() > 0 ? Cast<AMinimapCameraActor>(Found[0]) : nullptr;
}


void UGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    UpdatePlayerMarkers();
}

void UGameHUDWidget::OnDiceClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Dice Button Clicked!"));
    // TODO: 실제 주사위 로직 (예: PlayerController를 통해 서버 RPC 호출)
}

void UGameHUDWidget::OnItemUseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Item Use Button Clicked!"));
    // TODO: 아이템 사용 로직
}

void UGameHUDWidget::UpdateHP(float CurrentHp, float MaxHp)
{
    if (HP_Bar && MaxHp > 0.0f)
    {
        HP_Bar->SetPercent(CurrentHp / MaxHp);
    }

    if (Text_PlayerHP)
    {
        FText HPText = FText::Format(
            FText::FromString(TEXT("{0} / {1}")),
            FMath::FloorToInt(CurrentHp),
            FMath::FloorToInt(MaxHp)
        );
        Text_PlayerHP->SetText(HPText);
    }
}

void UGameHUDWidget::UpdatePlayerMarkers()
{
    TArray<UImage*> MarkerWidgets = { Img_PlayerMarker_0, Img_PlayerMarker_1, Img_PlayerMarker_2, Img_PlayerMarker_3 };

    for (UImage* M : MarkerWidgets)
    {
        if (M) M->SetVisibility(ESlateVisibility::Hidden);
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC || !PC->GetPawn()) continue;

        if (AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>())
        {
            int32 Idx = PS->PlayerNumber - 1;
            if (MarkerWidgets.IsValidIndex(Idx) && MarkerWidgets[Idx])
            {
                FVector Loc = PC->GetPawn()->GetActorLocation();

                float AlphaX = FMath::GetMappedRangeValueClamped(FVector2D(WorldMapMinX, WorldMapMaxX), FVector2D(0.f, 1.f), Loc.X);
                float AlphaY = FMath::GetMappedRangeValueClamped(FVector2D(WorldMapMinY, WorldMapMaxY), FVector2D(0.f, 1.f), Loc.Y);

                FVector2D MinimapPos(AlphaX * MinimapSize, AlphaY * MinimapSize);
                MarkerWidgets[Idx]->SetRenderTranslation(MinimapPos);
                MarkerWidgets[Idx]->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}