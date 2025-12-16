#include "MiniGame/Racing/T9_RacingGameState.h"
#include "MiniGame/Racing/T9_RacingGamePlayerController.h"

void AT9_RacingGameState::OnPhaseChanged(EMiniGamePhase NewPhase)
{
    UE_LOG(LogTemp, Error, TEXT("b"));
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AT9_RacingGamePlayerController* RPC = Cast<AT9_RacingGamePlayerController>(It->Get()))
        {
            RPC->ChangeUI(NewPhase);
        }
    }
}
