#include "Player/MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
	: CurrentHp(100),
	MaxHp(100),
	CurrentScore(0),
	TileIndex(0),
	PlayerID(0)
{

}

void AMyPlayerState::SetPlayerID(int Amount)
{
	PlayerID = Amount;
}

int AMyPlayerState::GetPlayerID()
{
	return PlayerID;
}

void AMyPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	AMyPlayerState* NewPS = Cast<AMyPlayerState>(NewPlayerState);
	if (NewPS)
	{
		//Passing by value
		//ex)NewPS->HP = HP;
		NewPS->PlayerNumber = PlayerNumber;
	}
}

void AMyPlayerState::SetHP(int Amount)
{
	CurrentHp = Amount;
}

int AMyPlayerState::GetHP()
{
	return CurrentHp;
}

void AMyPlayerState::SetMaxHP(int Amount)
{
	MaxHp = Amount;
}

int AMyPlayerState::GetMaxHP()
{
	return MaxHp;
}

void AMyPlayerState::AddHP(int Amount)
{
	CurrentHp += Amount;
}

void AMyPlayerState::SetScore(int Amount)
{
	CurrentScore = Amount;
}

int AMyPlayerState::GetScore()
{
	return CurrentScore;
}

void AMyPlayerState::AddScore(int Amount)
{
	CurrentScore += Amount;
}

void AMyPlayerState::SetTileIndex(int Amount)
{
	TileIndex = Amount;
}

int AMyPlayerState::GetTileIndex()
{
	return TileIndex;
}

int32 AMyPlayerState::GetPlayerNumber()
{
	return PlayerNumber;
}

void AMyPlayerState::SetPlayerNumber(int32 PNumber)
{
	PlayerNumber = PNumber;
}