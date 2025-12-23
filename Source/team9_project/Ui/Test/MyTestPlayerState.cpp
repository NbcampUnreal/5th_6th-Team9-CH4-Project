#include "MyTestPlayerState.h"
#include "Net/UnrealNetwork.h"

AMyTestPlayerState::AMyTestPlayerState()
{
    DisplayName = TEXT("Player");
    bIsReady = false;
    PlayerNumber = -1;
}
void AMyTestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMyTestPlayerState, DisplayName);
    DOREPLIFETIME(AMyTestPlayerState, bIsReady);
    DOREPLIFETIME(AMyTestPlayerState, PlayerNumber);
}