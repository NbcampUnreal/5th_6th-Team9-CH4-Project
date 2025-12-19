#include "MiniGame/Timing/T9_TimingGameState.h"
#include "MiniGame/Timing/T9_TimingGamePlayerController.h"
#include "Net/UnrealNetwork.h"
void AT9_TimingGameState::OnPhaseChanged(EMiniGamePhase NewPhase)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AT9_TimingGamePlayerController* RPC = Cast<AT9_TimingGamePlayerController>(It->Get()))
        {
            RPC->ChangeUI(NewPhase);
        }
    }
}

void AT9_TimingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AT9_TimingGameState, TargetTime);
    DOREPLIFETIME(AT9_TimingGameState, GameStartTime);
    DOREPLIFETIME(AT9_TimingGameState, Results);
}

void AT9_TimingGameState::AddPlayerPress(int32 PlayerId, float PressServerTime)
{
    for (auto& R : Results)
    {
        if (R.PlayerId == PlayerId)
            return;
    }

    int32 PressMs = int32((PressServerTime - GameStartTime) * 1000.f);
    int32 Delta = FMath::Abs(PressMs - TargetTime);

    Results.Add({ PlayerId, PressMs, Delta });
}

void AT9_TimingGameState::OnRep_Results()
{
    //Player UI Update 
}
