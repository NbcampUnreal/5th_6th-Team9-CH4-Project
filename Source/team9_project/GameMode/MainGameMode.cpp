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

int32 AMainGameMode::ThrowDice(const int8 MyTurnIndex)
{
	//차례가 아닌 플레이어는 주사위 못던진다.
	if (CheckPlayerTurn(MyTurnIndex))
	{
		return 0;
	}
	
	const int32 DiceNum = FMath::RandRange(1, 6);
	return DiceNum;
}

bool AMainGameMode::CheckPlayerTurn(const int8 TargetTurnIndex)
{
	return TurnIndex == TargetTurnIndex;
}

bool AMainGameMode::UsingItem(int8 MyTurnIndex, int32 InventoryIndex)
{
	//차례가 아닌 플레이어는 아이템 사용 불가능
	if (CheckPlayerTurn(MyTurnIndex))
	{
		return false;
	}

	//TODO : 이미 아이템 사용시 재사용 불가

	//TODO : 아이템 사용 구현
	
	return true;
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
		TurnIndex = 0;
		++CurrentRound;

		//TODO : 미니게임 시작 구현
	}
}
