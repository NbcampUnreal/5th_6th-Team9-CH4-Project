#include "GameMode/Team9GameInstance.h"

void UTeam9GameInstance::SetCurrentRound(int32 CurrentR)
{
	CurrentRound = CurrentR;
}

int32 UTeam9GameInstance::GetCurrentRound()
{
	return CurrentRound;
}

void UTeam9GameInstance::PropertyInit()
{
	CurrentRound = 1;
}
