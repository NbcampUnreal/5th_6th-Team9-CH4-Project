#include "MiniGame/Racing/T9_RacingGameState.h"
#include "MiniGame/Racing/T9_RacingGamePlayerController.h"
#include "Net/UnrealNetwork.h"

void AT9_RacingGameState::OnPhaseChanged(EMiniGamePhase NewPhase)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AT9_RacingGamePlayerController* RPC = Cast<AT9_RacingGamePlayerController>(It->Get()))
        {
            RPC->ChangeUI(NewPhase);
        }
    }
}

void AT9_RacingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AT9_RacingGameState, PlayerDistances);
}

void AT9_RacingGameState::UpdatePlayerDistance(int32 PlayerId, int32 Distance)
{
    if (!HasAuthority()) return;

    FRacingPlayerDistance* Found =
        PlayerDistances.Items.FindByPredicate(
            [&](const FRacingPlayerDistance& Item)
            {
                return Item.PlayerId == PlayerId;
            });

    if (!Found)
    {
        FRacingPlayerDistance NewItem;
        NewItem.PlayerId = PlayerId;
        NewItem.Distance = Distance;

        PlayerDistances.Items.Add(NewItem);
        PlayerDistances.MarkItemDirty(NewItem);
    }
    else
    {
        Found->Distance = Distance;
        PlayerDistances.MarkItemDirty(*Found);
    }
}

void FRacingPlayerDistance::PostReplicatedAdd(const FRacingPlayerDistanceArray& InArray)
{
    UE_LOG(LogTemp, Log,TEXT("[ADD] Player %d Distance %d"),PlayerId, Distance);
}

void FRacingPlayerDistance::PostReplicatedChange(const FRacingPlayerDistanceArray& InArray)
{
    UE_LOG(LogTemp, Log, TEXT("[CHANGE] Player %d Distance %d"), PlayerId, Distance);
}

void FRacingPlayerDistance::PreReplicatedRemove(const FRacingPlayerDistanceArray& InArray)
{
    UE_LOG(LogTemp, Log,TEXT("[REMOVE] Player %d"), PlayerId);
}
