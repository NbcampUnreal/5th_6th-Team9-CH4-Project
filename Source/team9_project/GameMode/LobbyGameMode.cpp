#include "GameMode/LobbyGameMode.h"
#include "Player/MyPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerNumber = 0;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerNumber = 0;
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	//번호 부여하기
	if (AMyPlayerState* MyPlayerState = NewPlayer->GetPlayerState<AMyPlayerState>())
	{
		//TODO : 번호 부여 기능 완성
	}

	PlayersInLobby.Add(NewPlayer);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayersInLobby.Remove(Exiting);
}

void ALobbyGameMode::SetPlayerName(AController* Exiting, const FString& NewPlayerName)
{
	//TODO : 이름 지정하기 구현
}

void ALobbyGameMode::MainGameStart()
{
	//TODO : 게임 시작 구현
}

int32 ALobbyGameMode::GivePlayerNumber()
{
	if (++PlayerNumber == MAX_int32)
	{
		PlayerNumber = 1;
	}
	
	return PlayerNumber; 
}
