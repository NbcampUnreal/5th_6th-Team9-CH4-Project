#include "MyTestLobbyGameMode.h"
#include "MyTestPlayerController.h"
#include "Ui/EGameUIState.h"
#include "Kismet/GameplayStatics.h"
#include "MyTestPlayerState.h"
#include "MyTestGameInstance.h"

AMyTestLobbyGameMode::AMyTestLobbyGameMode()
{
    PlayerStateClass = AMyTestPlayerState::StaticClass();

    bUseSeamlessTravel = true;
}

void AMyTestLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("PostLogin Called Player: %s"), *GetNameSafe(NewPlayer));

    if (NewPlayer->IsLocalController())  // 이 플레이어가 로컬 플레이어인지 체크 (클라이언트 측에서만 true)
    {
        if (AMyTestPlayerState* TestPS = NewPlayer->GetPlayerState<AMyTestPlayerState>())
        {
            if (UMyTestGameInstance* GI = Cast<UMyTestGameInstance>(GetWorld()->GetGameInstance()))
            {
                if (!GI->SavedPlayerName.IsEmpty() && GI->SavedPlayerName != TEXT("Player"))
                {
                    TestPS->DisplayName = GI->SavedPlayerName;
                    TestPS->SetPlayerName(GI->SavedPlayerName);

                    UE_LOG(LogTemp, Warning, TEXT("[PostLogin] Local Player Name Recoll: %s"), *GI->SavedPlayerName);
                }
            }
        }
    }

    // 기존 UI 설정 로직
    if (AMyTestPlayerController* PC = Cast<AMyTestPlayerController>(NewPlayer))
    {
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

FString AMyTestLobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    FString ErrorMessage = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

    FString JoinedName = UGameplayStatics::ParseOption(Options, TEXT("Name"));
    if (JoinedName.IsEmpty())
    {
        JoinedName = TEXT("Player");
    }

    if (AMyTestPlayerState* TestPS = NewPlayerController->GetPlayerState<AMyTestPlayerState>())
    {
        // 엔진 기본 이름 (옵션 없으면 랜덤 등)
        TestPS->SetPlayerName(JoinedName);

        // 우리가 쓰는 UI용 이름 (복제됨)
        TestPS->DisplayName = JoinedName;

        UE_LOG(LogTemp, Warning, TEXT("[GameMode] Option Input Name: %s"), *JoinedName);
        UE_LOG(LogTemp, Warning, TEXT("[GameMode] Final Current Name: %s"), *TestPS->DisplayName);
    }
    return ErrorMessage;
}