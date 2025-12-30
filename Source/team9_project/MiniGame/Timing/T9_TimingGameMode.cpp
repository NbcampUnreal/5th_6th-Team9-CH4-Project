#include "MiniGame/Timing/T9_TimingGameMode.h"
#include "MiniGame/Timing/T9_TimingGameState.h"

AT9_TimingGameMode::AT9_TimingGameMode()
{
}

void AT9_TimingGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_TimingGameMode::StartGame()
{
	Super::StartGame();
	UE_LOG(LogTemp, Error, TEXT("TimingStartGame"));
	TargetTimeMs = FMath::RandRange(5000, 10000);
	BestDeltaMs = int32(TargetTimeMs);
	GameStartTime = GetWorld()->GetTimeSeconds();
	AT9_TimingGameState* GS = GetGameState<AT9_TimingGameState>();
	if (GS)
	{
		GS->TargetTimeMs = TargetTimeMs;
		GS->GameStartTime = GameStartTime;
	}
	GetWorld()->GetTimerManager().SetTimer(
		TimeOverHandle,
		this,
		&AT9_TimingGameMode::OnTimeOver,
		TargetTimeMs / 1000.f,
		false
	);
}

void AT9_TimingGameMode::EndGame()
{
	Super::EndGame();
	UE_LOG(LogTemp, Error, TEXT("TimingEndGame"));
	GetWorld()->GetTimerManager().ClearTimer(TimeOverHandle);
	Winner();
	UE_LOG(LogTemp, Error, TEXT("Winner3"));
}

void AT9_TimingGameMode::OnTimeOver()
{
	EndGame();
}

void AT9_TimingGameMode::Winner()
{
	AT9_TimingGameState* GS = GetWorld()->GetGameState<AT9_TimingGameState>();
	int32 WinnerPlayer = 0;
	UE_LOG(LogTemp, Error, TEXT("Winner1 %d"), WinnerPlayer);
	for (const FTimingPlayerResult& R : GS->Results)
	{
		UE_LOG(LogTemp, Error, TEXT("Record %d"), R.DeltaMs);
		if (R.DeltaMs < BestDeltaMs)
		{
			BestDeltaMs = R.DeltaMs;
			WinnerPlayer = R.PlayerId;
			UE_LOG(LogTemp, Error, TEXT("Winner2 %d"), WinnerPlayer);
		}
		else if (R.DeltaMs == BestDeltaMs)//동등한기록
		{
			//고민중
			UE_LOG(LogTemp, Error, TEXT("Winner"));
		}
	}
	GS->WinnerPlayer = WinnerPlayer;
	return;
}
