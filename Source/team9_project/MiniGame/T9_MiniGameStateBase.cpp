#include "MiniGame/T9_MiniGameStateBase.h"
#include "Net/UnrealNetwork.h"

void AT9_MiniGameStateBase::AddPlayerResult(int32 PlayerId, int32 Score)
{
	FMiniGamePlayerResult Result;
	Result.PlayerId = PlayerId;
	Result.Score = Score;
	PlayerResults.Add(Result);
}

void AT9_MiniGameStateBase::SetPhase(EMiniGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase) return;
	UE_LOG(LogTemp, Error, TEXT("SetPhase"));
	CurrentPhase = NewPhase;
}

void AT9_MiniGameStateBase::OnRep_Phase()
{
	OnPhaseChanged(CurrentPhase);
	UE_LOG(LogTemp, Error, TEXT("OnRep_Phase"));
}

void AT9_MiniGameStateBase::OnPhaseChanged(EMiniGamePhase NewPhase)
{

}

void AT9_MiniGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT9_MiniGameStateBase, PlayerResults);
	DOREPLIFETIME(AT9_MiniGameStateBase, CurrentPhase);
}