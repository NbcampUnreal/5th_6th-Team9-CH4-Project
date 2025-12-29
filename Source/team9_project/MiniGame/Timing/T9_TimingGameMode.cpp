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
	TimerTime= FMath::RandRange(5000, 10000);
	BestDeltaMs = TimerTime;
	StartServerTime = GetWorld()->GetTimeSeconds();
	AT9_TimingGameState* GS = GetGameState<AT9_TimingGameState>();
	if (GS)
	{
		GS->TargetTime=TimerTime;
		GS->GameStartTime=StartServerTime;
	}
	GetWorld()->GetTimerManager().SetTimer(
		TimeOverHandle,
		this,
		&AT9_TimingGameMode::OnTimeOver,
		TimerTime / 1000.f,
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
	//ResultWidget업데이트
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
		UE_LOG(LogTemp, Error, TEXT("Record %f"), R.DeltaMs);
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
