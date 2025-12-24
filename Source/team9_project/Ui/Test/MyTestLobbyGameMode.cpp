#include "MyTestLobbyGameMode.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"
#include "Ui/EGameUIState.h"
#include "Kismet/GameplayStatics.h"
#include "MyTestGameInstance.h"

AMyTestLobbyGameMode::AMyTestLobbyGameMode()
{
    PlayerStateClass = AMyPlayerState::StaticClass();

    bUseSeamlessTravel = true;
}

void AMyTestLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("PostLogin Called Player: %s"), *GetNameSafe(NewPlayer));

    if (NewPlayer->IsLocalController())  // 이 플레이어가 로컬 플레이어인지 체크 (클라이언트 측에서만 true)
    {
        if (AMyPlayerState* TestPS = NewPlayer->GetPlayerState<AMyPlayerState>())
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
    if (AMyPlayerController* PC = Cast<AMyPlayerController>(NewPlayer))
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

    if (AMyPlayerState* TestPS = NewPlayerController->GetPlayerState<AMyPlayerState>())
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