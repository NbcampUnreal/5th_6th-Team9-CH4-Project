#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	virtual void Logout(AController* Exiting) override;

	//이름 지정하기
	void SetPlayerName(AController* Exiting, const FString& NewPlayerName);

private:
	//준비 완료된 플레이어 중 4명을 모아 게임 시작
	void MainGameStart();
	
	//플레이어에게 부여할 번호, 함수 호출마다 1씩 증가
	int32 GivePlayerNumber();
	
	//로비에 있는 플레이어 목록
	UPROPERTY()
	TSet<AController*> PlayersInLobby;

	//접속한 플레이어에게 부여할 번호
	int32 PlayerNumber;
};
