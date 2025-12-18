#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	virtual void Logout(AController* Exiting) override;

	//주사위 굴리기
	int32 ThrowDice(AActor* ThrowingPlayerActor);

	//지정한 플레이어의 턴인가
	bool CheckPlayerTurn(AActor* CheckPlayerActor);

	//현재 진행중인 턴인 플레이어 인덱스
	int8 GetTurnIndex();

private:
	//다음 플레이어 턴 시작
	void NextPlayerTurn();
	
	//게임중인 플레이어 목록 (턴 진행 순서대로)
	UPROPERTY()
	TArray<AController*> PlayersInGame;

	//현재 턴 진행중인 플레이어 번호
	int8 TurnIndex;

	//현재 라운드
	int16 CurrentRound;
};
