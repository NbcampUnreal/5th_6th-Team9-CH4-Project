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

	//게임 시작을 위해 준비된 플레이어를 찾을 때 사용할 타이머 핸들
	FTimerHandle TimerHandleForStart;

	//이 시간마다 준비 완료된 플레이어를 찾아 게임을 시작한다.
	UPROPERTY(EditInstanceOnly, Category = "Game Start", meta = (AllowPrivateAccess = true))
	float StartGameDelay;
	
	//로비에 있는 플레이어 목록
	UPROPERTY()
	TMap<int32, AController*> PlayersInLobby;

	//접속한 플레이어에게 부여할 번호
	int32 PlayerNumber;

	//메인 게임 맵 이름
	FName MAIN_GAME_MAP_NAME = TEXT("Main");
};
