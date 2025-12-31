#include "GameMode/LobbyGameMode.h"
#include "Team9GameInstance.h"
#include "Player/MyPlayerState.h"

class UTeam9GameInstance;

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
	
	PlayerNumber = 0;
	StartGameDelay = 10.0f;
	NeedPlayers = 4;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerNumber = 0;

	GetWorld()->GetTimerManager().SetTimer(TimerHandleForStart, this, &ALobbyGameMode::MainGameStart, StartGameDelay, true);
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	//번호 부여하기
	int32 NewNumber = GivePlayerNumber();
	if (AMyPlayerState* MyPlayerState = NewPlayer->GetPlayerState<AMyPlayerState>())
	{
		MyPlayerState->SetPlayerNumber(NewNumber);
	}

	PlayersInLobby.Add(NewNumber, NewPlayer);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (AMyPlayerState* MyPlayerState = Exiting->GetPlayerState<AMyPlayerState>())
	{
		PlayersInLobby.Remove(MyPlayerState->GetPlayerNumber());
	}
}

void ALobbyGameMode::SetPlayerName(int32 TargetPlayerNumber, const FString& NewPlayerName)
{
	if (!PlayersInLobby.Find(TargetPlayerNumber))
	{
		return;
	}

	AMyPlayerState* MyPlayerState = PlayersInLobby[TargetPlayerNumber]->GetPlayerState<AMyPlayerState>();
	if (!IsValid(MyPlayerState))
	{
		return;
	}

	MyPlayerState->DisplayName = NewPlayerName;
}

void ALobbyGameMode::MainGameStart()
{
	//준비 완료된 플레이어를 지정한 수가 될 때까지 찾는다.
	TArray<AMyPlayerState*> LobbyPlayerStates;
	for (auto PlayerInfo : PlayersInLobby)
	{
		AMyPlayerState* MyPlayerState = PlayerInfo.Value->GetPlayerState<AMyPlayerState>();
		if (!IsValid(MyPlayerState))
		{
			continue;
		}

		if (MyPlayerState->bIsReady)
		{
			LobbyPlayerStates.Add(MyPlayerState);
			if (LobbyPlayerStates.Num() >= NeedPlayers)
			{
				break;
			}
		}
	}

	if (LobbyPlayerStates.Num() < NeedPlayers)
	{
		return;
	}

	//메인 게임 맵으로 이동
	if (UTeam9GameInstance* GameInstance = GetWorld()->GetGameInstance<UTeam9GameInstance>())
	{
		GameInstance->PropertyInit();
	}

	GetWorld()->ServerTravel(MAIN_GAME_MAP_NAME);
}

int32 ALobbyGameMode::GivePlayerNumber()
{
	if (++PlayerNumber == MAX_int32)
	{
		PlayerNumber = 1;
	}

	return PlayerNumber;
}