#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundEnd);

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

	//(제거 예정)
	UFUNCTION(meta = (DeprecatedFunction, DeprecationMessage = "Use ThrowDice(int8) instead."))
	int32 ThrowDice(AController* Controller);

	//주사위 굴리기 (턴 플레이어 체크)
	int32 ThrowDice(const int32 MyPlayerNumber);

	//주사위 굴리기 (무조건 굴림)
	int32 ThrowDice();

	//지정한 플레이어의 턴인가
	bool CheckPlayerTurn(const int32 MyPlayerNumber);

	//아이템 사용하기
	bool UsingItem(const int32 MyPlayerNumber, const int32 InventoryIndex);

	//현재 진행중인 턴인 플레이어 인덱스
	int8 GetTurnPlayerNumber();
	
private:
	//플레이어 순서 정하기
	void SetPlayerNumbersOrder();
	
	//다음 플레이어 턴 시작
	void NextPlayerTurn();

public:
	//라운드 종료시 실행할 델리게이트
	FOnRoundEnd OnRoundEnd;
	
private:
	//게임중인 플레이어 목록
	UPROPERTY()
	TMap<int32, AController*> PlayersInGame;

	//턴 진행 순서
	TArray<int32> OrderedPlayerNumbers;

	//현재 턴 진행중인 플레이어 번호
	int32 TurnPlayerNumber;

	//현재 턴 진행중인 플레이어 인덱스
	int32 TurnIndex;

	//현재 라운드
	int16 CurrentRound;
};
