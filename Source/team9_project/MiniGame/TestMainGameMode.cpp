#include "MiniGame/TestMainGameMode.h"
#include "Player/MyPlayerState.h"

ATestMainGameMode::ATestMainGameMode()
{
	bUseSeamlessTravel = true;
}

void ATestMainGameMode::ExecuteAfter5Seconds()
{
	GetWorld()->ServerTravel(TEXT("/Game/MiniGame/Maps/RapidInputRacingMinigame?listen"), true);
}

void ATestMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerNumber = 0;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_DelayedAction,
		this,
		&ATestMainGameMode::ExecuteAfter5Seconds,
		5.0f,
		false
	);
}

void ATestMainGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	//번호 부여하기
	if (AMyPlayerState* MyPlayerState = NewPlayer->GetPlayerState<AMyPlayerState>())
	{
		MyPlayerState->SetPlayerNumber(GivePlayerNumber());
	}

	PlayersInLobby.Add(NewPlayer);
}

void ATestMainGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayersInLobby.Remove(Exiting);
}

int32 ATestMainGameMode::GivePlayerNumber()
{
	if (++PlayerNumber == MAX_int32)
	{
		PlayerNumber = 1;
	}

	return PlayerNumber;
}