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
    if (!GS)
    {
        return;
    }
    float Elapsed = GetWorld()->GetTimeSeconds() - GS->GameStartTime;
    float Remaining = (GS->TargetTime / 1000.f) - Elapsed;

    CountdownText->SetText(FText::AsNumber(FMath::Max(0.f, Remaining)));
}

void UT9_TimingInGameWidget::OnStopClicked()
{
    StopButton->SetIsEnabled(false);
    UE_LOG(LogTemp, Log, TEXT("Click"));
    AT9_TimingGamePlayerController* PC = Cast<AT9_TimingGamePlayerController>(GetOwningPlayer());

    if (PC)
    {
        PC->ServerRPC_StopTimer();
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
