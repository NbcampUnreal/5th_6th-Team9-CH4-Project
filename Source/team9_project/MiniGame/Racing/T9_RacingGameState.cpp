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

FText AT9_RacingGameState::GetGameName()
{
    return GameName;
}

FText AT9_RacingGameState::GetGameDescription()
{
    return GameDescription;
}

UTexture2D* AT9_RacingGameState::GetGameTexture()
{
    return GameTexture;
}
