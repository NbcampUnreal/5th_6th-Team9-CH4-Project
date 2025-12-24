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

private:
	int32 CurrentRound;

	// Cho_Sungmin - 플레이어별 저장 데이터
	UPROPERTY()
	TMap<int32, FPlayerPersistentData> PlayerDataMap;
};
