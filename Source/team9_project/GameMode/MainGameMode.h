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

	//(제거 예정) 주사위 굴리기
	UFUNCTION(meta = (DeprecatedFunction, DeprecationMessage = "Use ThrowDice(int8) instead."))
	int32 ThrowDice(AController* Controller);

	//주사위 굴리기
	int32 ThrowDice(const int8 MyTurnIndex);

	//지정한 플레이어의 턴인가
	bool CheckPlayerTurn(const int8 TargetTurnIndex);

	//아이템 사용하기
	bool UsingItem(int8 MyTurnIndex, int32 InventoryIndex);

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
