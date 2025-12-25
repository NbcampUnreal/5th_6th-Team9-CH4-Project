#include "GameMode/LobbyGameMode.h"
#include "Team9GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"

class UTeam9GameInstance;

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
	int32 NewNumber = GivePlayerNumber();
	if (AMyPlayerState* MyPlayerState = NewPlayer->GetPlayerState<AMyPlayerState>())
	{
		MyPlayerState->SetPlayerNumber(NewNumber);
	}

	PlayersInLobby.Add(NewNumber, NewPlayer);

	//4인 이상이면 게임 시작
	if (PlayersInLobby.Num() >= 4)
	{
		MainGameStart();
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (AMyPlayerState* MyPlayerState = Exiting->GetPlayerState<AMyPlayerState>())
	{
		PlayersInLobby.Remove(MyPlayerState->GetPlayerNumber());
	}
}

void ALobbyGameMode::SetPlayerName(AController* Exiting, const FString& NewPlayerName)
{
	//TODO : 이름 지정하기 구현
}

void ALobbyGameMode::MainGameStart()
{
	if (UTeam9GameInstance* GameInstance = GetWorld()->GetGameInstance<UTeam9GameInstance>())
	{
		GameInstance->PropertyInit();
	}
	UGameplayStatics::OpenLevel(this, TEXT("Main"));
}

int32 ALobbyGameMode::GivePlayerNumber()
{
	if (++PlayerNumber == MAX_int32)
	{
		PlayerNumber = 1;
	}
	
	return PlayerNumber; 
}
