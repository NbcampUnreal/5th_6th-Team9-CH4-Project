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

    DOREPLIFETIME(AT9_TimingGameState, TargetTimeMs);
    DOREPLIFETIME(AT9_TimingGameState, GameStartTime);
    DOREPLIFETIME(AT9_TimingGameState, Results);
}

void AT9_TimingGameState::AddPlayerPress(int32 PlayerId, float PressServerTime)
{
    for (const FTimingPlayerResult& R : Results)
    {
        if (R.PlayerId == PlayerId)
            return;
    }

    int32 PressMs = int32((PressServerTime - GameStartTime)*1000.f);
    int32 Delta = FMath::Abs(PressMs - TargetTimeMs);

    Results.Add({ PlayerId, PressMs, Delta });

    UE_LOG(LogTemp, Log,
        TEXT("[AddPlayerPress] Player=%d PressMs=%d Target=%d Delta=%d ResultsCount=%d"),
        PlayerId,
        PressMs,
        TargetTimeMs,
        Delta,
        Results.Num());
}

FText AT9_TimingGameState::GetGameName()
{
    return GameName;
}

FText AT9_TimingGameState::GetGameDescription()
{
    return GameDescription;
}

UTexture2D* AT9_TimingGameState::GetGameTexture()
{
    return GameTexture;
}

void AT9_TimingGameState::OnRep_Results()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        AT9_TimingGamePlayerController* PC =
            Cast<AT9_TimingGamePlayerController>(It->Get());

        if (!PC) continue;

        PC->NotifyTimingResultUpdated();
    }
}