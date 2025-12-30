#include "MiniGame/Timing/T9_TimingInGameWidget.h"
#include "MiniGame/Timing/T9_TimingGameState.h"
#include "MiniGame/Timing/T9_TimingGamePlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UT9_TimingInGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StopButton)
    {
        StopButton->OnClicked.AddDynamic(this, &UT9_TimingInGameWidget::OnStopClicked);
    }
    PlayAnimation(FadeOutCountdown);
}

void UT9_TimingInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    AT9_TimingGameState* GS = GetWorld()->GetGameState<AT9_TimingGameState>();
    if (!GS) return;

    float ElapsedSec = GetWorld()->GetTimeSeconds() - GS->GameStartTime;

    int32 ElapsedMs = int32(ElapsedSec * 1000.f);
    int32 RemainMs = FMath::Max(0, GS->TargetTimeMs - ElapsedMs);

    float RemainSec = RemainMs / 1000.f;

    CountdownText->SetText(FText::AsNumber(FMath::Max(0.f, RemainSec)));
}

void UT9_TimingInGameWidget::OnStopClicked()
{
    StopButton->SetIsEnabled(false);
    UE_LOG(LogTemp, Log, TEXT("Click"));
    AT9_TimingGamePlayerController* PC = Cast<AT9_TimingGamePlayerController>(GetOwningPlayer());

    if (PC)
    {
        PC->ServerRPC_StopTimer(GetWorld()->GetTimeSeconds());
    }
}

void UT9_TimingInGameWidget::UpdateRankingUI()
{
    AT9_TimingGameState* GS = GetWorld()->GetGameState<AT9_TimingGameState>();
    if (!GS)
    {
        return;
    }
    if (StopCheck)
    {
        FString StopString;
        for (int32 i = 0; i < GS->Results.Num(); i++)
        {
            StopString += FString::Printf(
                TEXT("Player %d : Stop\n"),
                GS->Results[i].PlayerId
            );
        }
        StopCheck->SetText(FText::FromString(StopString));
    }
}
