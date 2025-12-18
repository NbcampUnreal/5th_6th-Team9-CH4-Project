#include "Player/MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
	: CurrentHp(100),
	MaxHp(100),
	CurrentScore(0)
{
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
