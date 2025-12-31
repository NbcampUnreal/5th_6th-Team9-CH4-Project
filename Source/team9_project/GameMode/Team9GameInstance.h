#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/Data/ItemData.h"
#include "Team9GameInstance.generated.h"

UCLASS()
class TEAM9_PROJECT_API UTeam9GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 게임 시작 시 자동 실행 → UI 서브시스템에 위젯 등록
	virtual void Init() override;
	void Callback_PostLoadMapWithWorld(UWorld* inWorld);
	void SetCurrentRound(int32 CurrentR);

	int32 GetCurrentRound();

	void PropertyInit();

	// Cho_Sungmin - 플레이어 데이터 저장/로드 (맵 이동 시 유지)
	UFUNCTION(BlueprintCallable, Category = "PlayerData")
	void SavePlayerData(int32 PlayerID, const FPlayerPersistentData& Data);

	UFUNCTION(BlueprintCallable, Category = "PlayerData")
	FPlayerPersistentData LoadPlayerData(int32 PlayerID) const;

	UFUNCTION(BlueprintCallable, Category = "PlayerData")
	bool HasPlayerData(int32 PlayerID) const;

	UFUNCTION(BlueprintCallable, Category = "PlayerData")
	void ClearPlayerData(int32 PlayerID);

	UFUNCTION(BlueprintCallable, Category = "PlayerData")
	void ClearAllPlayerData();

	void SetTurnOrderedPlayerNums(TArray<int32> ArrayValue);

	TArray<int32> GetTurnOrderedPlayerNums();

private:
	int32 CurrentRound;

	// Cho_Sungmin - 플레이어별 저장 데이터
	UPROPERTY()
	TMap<int32, FPlayerPersistentData> PlayerDataMap;

	//메인 게임 모드에서 사용하는 턴 플레이어 순서
	TArray<int32> TurnOrderedPlayerNums;
};
