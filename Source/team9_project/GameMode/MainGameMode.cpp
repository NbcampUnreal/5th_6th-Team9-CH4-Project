#include "MainGameMode.h"
#include "Team9GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"

AMainGameMode::AMainGameMode()
{
	bUseSeamlessTravel = true;
	TurnIndex = 0;
	CurrentRound = 0;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	UTeam9GameInstance* GI = GetWorld()->GetGameInstance<UTeam9GameInstance>();
	if (GI && HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GI CurrentTurn = %d"), GI->GetCurrentRound());
		CurrentRound = GI->GetCurrentRound();
	}
	TurnIndex = 0;

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

void AMainGameMode::NextPlayerTurn()
{
	//턴 인덱스를 1 추가하는데 마지막 플레이어가 진행하면 현재 라운드 종료 미니게임 시작
	if (PlayersInGame.Num() > ++TurnIndex)
	{
		TurnIndex = 0;
		++CurrentRound;

		OnRoundEnd.Broadcast();

		UGameplayStatics::OpenLevel(this, TEXT("MiniGame"));

		return;
	}

	//아직 라운드 종료가 아니라면 다음 플레이어의 차례 진행
	TurnPlayerNumber = OrderedPlayerNumbers[TurnIndex];
}
