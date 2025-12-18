#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{
	TurnIndex = 0;
	CurrentRound = 0;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TurnIndex = 0;
	CurrentRound = 1;
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

int32 AMainGameMode::ThrowDice(AActor* ThrowingPlayerActor)
{
	const int32 DiceNum = FMath::RandRange(1, 6);
	return DiceNum;
}

bool AMainGameMode::CheckPlayerTurn(AActor* CheckPlayerActor)
{
	AController* Owner = Cast<AController>(CheckPlayerActor->GetOwner());
	if (!Owner)
	{
		return false;
	}
	
	return PlayersInGame[TurnIndex] == Owner;
}

int8 AMainGameMode::GetTurnIndex()
{
	return TurnIndex;
}

void AMainGameMode::NextPlayerTurn()
{
	//턴 인덱스를 1 추가하는데 마지막 플레이어가 진행하면 라운드 증가 미니게임 시작
	if (PlayersInGame.Num() > ++TurnIndex)
	{
		++CurrentRound;

		//TODO : 미니게임 시작 구현
	}
}
