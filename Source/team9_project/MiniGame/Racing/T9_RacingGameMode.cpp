#include "MiniGame/Racing/T9_RacingGameMode.h"


AT9_RacingGameMode::AT9_RacingGameMode()
{
}

void AT9_RacingGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_RacingGameMode::StartGame()
{
	Super::StartGame();
	UE_LOG(LogTemp, Error, TEXT("StartRacingGame"));
}

void AT9_RacingGameMode::EndGame()
{
	Super::EndGame();
	UE_LOG(LogTemp, Error, TEXT("EndRacingGame"));
	UE_LOG(LogTemp, Error, TEXT("Winner %d"), Winner);
}
