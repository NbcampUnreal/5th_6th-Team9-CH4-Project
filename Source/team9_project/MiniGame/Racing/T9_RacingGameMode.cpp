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
	UE_LOG(LogTemp, Error, TEXT("StartGame"));
}

void AT9_RacingGameMode::EndGame()
{

}