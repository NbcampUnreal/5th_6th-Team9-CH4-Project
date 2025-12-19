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
}

void AT9_TimingGameMode::OnTimeOver()
{
	EndGame();
}
