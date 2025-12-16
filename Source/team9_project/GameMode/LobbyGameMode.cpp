#include "GameMode/LobbyGameMode.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

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
