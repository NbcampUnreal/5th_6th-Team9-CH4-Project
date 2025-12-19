#include "MiniGame/Timing/T9_TimingInGameWidget.h"
#include "MiniGame/Timing/T9_TimingGameState.h"
#include "MiniGame/Timing/T9_TimingGamePlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

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

    AT9_TimingGamePlayerController* PC = Cast<AT9_TimingGamePlayerController>(GetOwningPlayer());

    if (PC)
    {
        PC->ServerRPC_StopTimer();
    }
}
