#include "MiniGame/T9_MiniGameModeBase.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "Net/UnrealNetwork.h"

AT9_MiniGameModeBase::AT9_MiniGameModeBase()
{
	SetPhase(EMiniGamePhase::None);
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
	GetWorld()->GetTimerManager().SetTimer(
		ComeBackHandle,
		this,
		&AT9_MiniGameModeBase::ComeBackMainGame,
		3.0f,
		false
	);
}

bool AT9_MiniGameModeBase::CheckPlayer()
{
	//All Player Ready Check
	return true;
}

void AT9_MiniGameModeBase::CheckStartGame()
{
	if (CheckPlayer()) 
	{	
		StartGame();
	}
}

void AT9_MiniGameModeBase::ComeBackMainGame()
{
	UE_LOG(LogTemp, Error, TEXT("SeverTravel"));
}

void AT9_MiniGameModeBase::SetPhase(EMiniGamePhase NewPhase)
{
	if (AT9_MiniGameStateBase* GS = GetGameState<AT9_MiniGameStateBase>())
	{
		GS->SetPhase(NewPhase);
	}
}

