#include "MainGameMode.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayersInGame.Add(NewPlayer);
}

void AMainGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayersInGame.Remove(Exiting);
}

int32 AMainGameMode::ThrowDice(AController* ThrowingPlayer)
{
	const int32 DiceNum = FMath::RandRange(1, 6);
	return DiceNum;
}
