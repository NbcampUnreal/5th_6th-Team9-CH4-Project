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
	AT9_MiniGameStateBase* GS = GetGameState<AT9_MiniGameStateBase>();
	if (GS)
	{
		GS->InitializePlayerReadys();
		for (int32 i = 0; i < GS->PlayerReadys.Num(); ++i)
		{
			const FMiniGamePlayerReady& Info = GS->PlayerReadys[i];
			UE_LOG(LogTemp, Warning,
				TEXT("[%d] PlayerNumber=%d | bReady=%d"),
				i,
				Info.PlayerId,
				Info.bReady
			);
		}
	}
	//UI최초 업데이트 리플리케이션 OnRep직접 호출해야함
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
		5.0f,
		false
	);
}

bool AT9_MiniGameModeBase::CheckPlayer()
{
	AT9_MiniGameStateBase* GS = GetWorld()->GetGameState<AT9_MiniGameStateBase>();
	if (GS->PlayerReadys.Num() == 0)
	{
		return false;
	}

	for (const FMiniGamePlayerReady& Info : GS->PlayerReadys)
	{
		if (!Info.bReady)
		{
			return false;
		}
	}

	return true;
}

void AT9_MiniGameModeBase::CheckStartGame()
{
	UE_LOG(LogTemp, Error, TEXT("CheckGame"));
	if (CheckPlayer()) 
	{	
		StartGame();
	}
}

void AT9_MiniGameModeBase::ComeBackMainGame()
{
	UE_LOG(LogTemp, Error, TEXT("SeverTravel"));
	//GetWorld()->ServerTravel(TEXT("/Game/MiniGame/Maps/RapidInputRacingMinigame.RapidInputRacingMinigame?listen"), true);
}

void AT9_MiniGameModeBase::SetPhase(EMiniGamePhase NewPhase)
{
	if (AT9_MiniGameStateBase* GS = GetGameState<AT9_MiniGameStateBase>())
	{
		GS->SetPhase(NewPhase);
	}
}

