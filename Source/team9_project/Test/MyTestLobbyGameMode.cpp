#include "MyTestLobbyGameMode.h"
#include "MyTestPlayerController.h"
#include "Ui/EGameUIState.h"

AMyTestLobbyGameMode::AMyTestLobbyGameMode()
{
    bUseSeamlessTravel = true;
}

void AMyTestLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("PostLogin 호출됨 플레이어: %s"), *GetNameSafe(NewPlayer));

    if (AMyTestPlayerController* PC = Cast<AMyTestPlayerController>(NewPlayer))
    {
        UE_LOG(LogTemp, Warning, TEXT("Client_SetUIState(InGame) 호출 시도"));
        // 클라이언트가 준비될 때까지 살짝 딜레이
        FTimerHandle Handle;
        GetWorldTimerManager().SetTimer(Handle, [PC]()
            {
                if (PC && PC->IsValidLowLevel())
                {
                    PC->Client_SetUIState(EGameUIState::Lobby);
                }
            }, 0.3f, false);
    }
}