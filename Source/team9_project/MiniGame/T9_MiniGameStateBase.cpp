#include "MiniGame/T9_MiniGameStateBase.h"
#include "Net/UnrealNetwork.h"

void AT9_MiniGameStateBase::AddPlayerResult(int32 PlayerId, int32 Score)
{
	FMiniGamePlayerResult Result;
	Result.PlayerId = PlayerId;
	Result.Score = Score;
	PlayerResults.Add(Result);
}

void AT9_MiniGameStateBase::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT9_MiniGameStateBase, PlayerResults);
}