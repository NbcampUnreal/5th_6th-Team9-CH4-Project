#include "MainGameMode.h"
#include "Team9GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "Player/MyPlayerController.h"

AMainGameMode::AMainGameMode()
{
	bUseSeamlessTravel = true;
	TurnPlayerNumber = 0;
	TurnIndex = 0;
	CurrentRound = 0;
	FirstReadyCheckTime = 1.0f;
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

	//1라운드(처음 시작)인 경우 진행 순서 정하고 준비를 기다린다.
	if (CurrentRound <= 1)
	{
		WaitForReady();
		SetPlayerNumbersOrder();
		
		return;
	}

	//라운드 시작
	NextPlayerTurn(true);
}

void AMainGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(NewPlayer);
	if (!IsValid(MyPlayerController))
	{
		return;
	}
	
	AMyPlayerState* MyPlayerState = MyPlayerController->GetPlayerState<AMyPlayerState>();
	if (IsValid(MyPlayerState))
	{
		PlayersInGame.Add(MyPlayerState->GetPlayerNumber(), MyPlayerController);
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

	//주사위 결과를 각 플레이어 컨트롤러에게 전달
	for (auto PlayerInfo : PlayersInGame)
	{
		PlayerInfo.Value->Client_ReceiveDiceResult(MyPlayerNumber, DiceNum);
	}
	
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
	//1 ~ 6의 숫자를 무작위로 섞기
	TArray DiceNums = { 1, 2, 3, 4, 5, 6 };
	for (int32 iNum = 5; iNum > 0 ; --iNum)
	{
		if (int32 RandomIndex = FMath::RandRange(0, iNum); iNum != RandomIndex)
		{
			DiceNums.Swap(iNum, RandomIndex);
		}
	}

	//앞에서 부터 각 플레이어에게 배정
	int8 ArrayIndex = -1;
	TMap<int32, int32> OrderByDiceNum;
	TArray<int32> SendPlayerNumbers;
	TArray<int32> SendDiceNums;
	for (auto PlayerInfo : PlayersInGame)
	{
		OrderByDiceNum.Add(DiceNums[++ArrayIndex], PlayerInfo.Key);
		SendPlayerNumbers.Add(PlayerInfo.Key);
		SendDiceNums.Add(DiceNums[ArrayIndex]);
	}

	//순서 정보를 각 플레이어에게 전달
	for (auto PlayerInfo : PlayersInGame)
	{
		PlayerInfo.Value->Client_ReceiveFirstOrder(SendPlayerNumbers, SendDiceNums);
	}

	//배정받은 수 기준 내림차순으로 순서가 정해진다.
	for (int32 iNum = 6; iNum >= 1; --iNum)
	{
		if (OrderByDiceNum.Contains(iNum))
		{
			OrderedPlayerNumbers.Add(OrderByDiceNum[iNum]);
		}
	}
}

void AMainGameMode::WaitForReady()
{
	//모든 플레이어의 준비를 해제
	TArray<AMyPlayerState*> PlayerStates;//플레이어 스테이트 따로 빼기
	for (auto PlayerInfo : PlayersInGame)
	{
		AMyPlayerState* MyPlayerState = PlayerInfo.Value->GetPlayerState<AMyPlayerState>();
		if (!IsValid(MyPlayerState))
		{
			UE_LOG(LogTemp, Error, TEXT("Find error while wait for ready."));
			return;
		}

		MyPlayerState->bIsReady = false;
		PlayerStates.Add(MyPlayerState);
	}
	
	//모두의 준비 완료까지 계속 확인한다.
	GetWorld()->GetTimerManager().SetTimer(FirstReadyHandle, FTimerDelegate::CreateLambda([&]()
	{
		bool bGameReady = true;
		for (AMyPlayerState* MyPlayerState : PlayerStates)
		{
			if (!MyPlayerState->bIsReady)
			{
				bGameReady = false;
				break;
			}
		}

		//모두의 준비 완료시 게임 시작
		if (bGameReady)
		{
			GetWorld()->GetTimerManager().ClearTimer(FirstReadyHandle);
			NextPlayerTurn(true);
		}
	}), FirstReadyCheckTime, true);
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

	//잠시후 미니게임 시작
	GetWorld()->GetTimerManager().SetTimer(WaitForMiniGameHandle, FTimerDelegate::CreateLambda([&]()
	{
		//TODO : 미니게임 시작 알림
	
		//무작위 미니게임맵 하나 사용
		int32 RandomIndex = FMath::RandRange(0, MiniGameMapNames.Num() - 1);
		UGameplayStatics::OpenLevel(this, MiniGameMapNames[RandomIndex]);
		
	}), MiniGameWaitTime, false);
}
