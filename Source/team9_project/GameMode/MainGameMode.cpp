#include "MainGameMode.h"
#include "Team9GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"

AMainGameMode::AMainGameMode()
{
	bUseSeamlessTravel = true;
	TurnPlayerNumber = 0;
	TurnIndex = 0;
	CurrentRound = 0;
	MiniGameWaitTime = 1.0f;
	MaxRound = 5;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTeam9GameInstance* GameInstance = GetWorld()->GetGameInstance<UTeam9GameInstance>())
	{
		CurrentRound = GameInstance->GetCurrentRound();
	}
	TurnPlayerNumber = 0;
	TurnIndex = 0;

	//마지막 라운드를 초과한 경우 게임 종료
	if (CurrentRound > MaxRound)
	{
		return;
	}

	//1라운드(처음 시작)인 경우 진행 순서 정하기
	if (CurrentRound == 1)
	{
		SetPlayerNumbersOrder();
		return;
	}

	//라운드 시작
	NextPlayerTurn(true);
}

void AMainGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (AMyPlayerState* MyPlayerState = NewPlayer->GetPlayerState<AMyPlayerState>())
	{
		PlayersInGame.Add(MyPlayerState->GetPlayerNumber(), NewPlayer);
	}
}

void AMainGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (AMyPlayerState* MyPlayerState = Exiting->GetPlayerState<AMyPlayerState>())
	{
		PlayersInGame.Remove(MyPlayerState->GetPlayerNumber());
	}
}

int32 AMainGameMode::ThrowDice(AController* Controller)
{
	return 1;
}

int32 AMainGameMode::ThrowDice(const int32 MyPlayerNumber)
{
	//차례가 아닌 플레이어는 주사위 못던진다.
	if (CheckPlayerTurn(MyPlayerNumber))
	{
		return 0;
	}
	
	const int32 DiceNum = FMath::RandRange(1, 6);
	return DiceNum;
}

int32 AMainGameMode::ThrowDice()
{
	const int32 DiceNum = FMath::RandRange(1, 6);
	return DiceNum;
}

bool AMainGameMode::CheckPlayerTurn(const int32 MyPlayerNumber)
{	
	return TurnPlayerNumber == MyPlayerNumber;
}

bool AMainGameMode::UsingItem(const int32 MyPlayerNumber, const int32 InventoryIndex)
{
	//차례가 아닌 플레이어는 아이템 사용 불가능
	if (CheckPlayerTurn(MyPlayerNumber))
	{
		return false;
	}

	//TODO : 이미 아이템 사용시 재사용 불가

	//TODO : 아이템 사용 구현
	
	return true;
}

int8 AMainGameMode::GetTurnPlayerNumber()
{
	return TurnPlayerNumber;
}

void AMainGameMode::SetPlayerNumbersOrder()
{
	//TODO : 플레이어 순서 정하기
	//임시로 TMap 기준 순서 따름
	for (auto PlayerInfo : PlayersInGame)
	{
		OrderedPlayerNumbers.Add(PlayerInfo.Key);
	}
}

void AMainGameMode::WaitForReady()
{
	//TODO : 각 플레이어의 준비 기다림
}

void AMainGameMode::NextPlayerTurn(bool bRoundStart)
{
	//라운드 시작시 첫 번째 플레이어를 지정
	if (bRoundStart)
	{
		TurnIndex = 0;
	}
	//턴 인덱스를 1 추가하는데 마지막 플레이어가 진행하면 현재 라운드 종료 미니게임 시작
	else if (PlayersInGame.Num() > ++TurnIndex)
	{
		++CurrentRound;
		OnRoundEnd.Broadcast();

		MoveToMiniGameMap();
		return;
	}

	//아직 라운드 종료가 아니라면 다음 플레이어의 차례 진행
	TurnPlayerNumber = OrderedPlayerNumbers[TurnIndex];
}

void AMainGameMode::MoveToMiniGameMap()
{
	//인스턴스에 현재 라운드 저장
	if (UTeam9GameInstance* GameInstance = GetWorld()->GetGameInstance<UTeam9GameInstance>())
	{
		 GameInstance->SetCurrentRound(CurrentRound);
	}

	//TODO : 잠시후 미니게임 시작
	
}
