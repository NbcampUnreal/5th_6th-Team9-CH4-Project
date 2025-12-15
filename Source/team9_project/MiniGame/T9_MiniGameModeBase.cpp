#include "MiniGame/T9_MiniGameModeBase.h"
#include "Net/UnrealNetwork.h"

AT9_MiniGameModeBase::AT9_MiniGameModeBase()
{
	CurrentPhase = EMiniGamePhase::Ready;
}

void AT9_MiniGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetPhase(EMiniGamePhase::Ready);
}

void AT9_MiniGameModeBase::StartGame()
{
	SetPhase(EMiniGamePhase::Playing);
}

void AT9_MiniGameModeBase::EndGame()
{
	SetPhase(EMiniGamePhase::Result);
}

bool AT9_MiniGameModeBase::CheckGame()
{
	return false;
}

void AT9_MiniGameModeBase::SetPhase(EMiniGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase) return;

	CurrentPhase = NewPhase;
	OnPhaseChanged(NewPhase);
}

void AT9_MiniGameModeBase::OnRep_Phase()
{
	OnPhaseChanged(CurrentPhase);
}

void AT9_MiniGameModeBase::OnPhaseChanged(EMiniGamePhase NewPhase)
{
	
}

void AT9_MiniGameModeBase::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT9_MiniGameModeBase, CurrentPhase);
}