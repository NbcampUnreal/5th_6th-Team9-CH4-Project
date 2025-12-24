#include "GameMode/Team9GameInstance.h"

void UTeam9GameInstance::SetCurrentRound(int32 CurrentR)
{
	CurrentRound = CurrentR;
}

int32 UTeam9GameInstance::GetCurrentRound()
{
	return CurrentRound;
}

void UTeam9GameInstance::PropertyInit()
{
	CurrentRound = 1;
}

// Cho_Sungmin - 플레이어 데이터 저장/로드 구현
void UTeam9GameInstance::SavePlayerData(int32 PlayerID, const FPlayerPersistentData& Data)
{
	PlayerDataMap.Add(PlayerID, Data);
	UE_LOG(LogTemp, Log, TEXT("GameInstance: Saved player data for PlayerID %d"), PlayerID);
}

FPlayerPersistentData UTeam9GameInstance::LoadPlayerData(int32 PlayerID) const
{
	if (const FPlayerPersistentData* Found = PlayerDataMap.Find(PlayerID))
	{
		UE_LOG(LogTemp, Log, TEXT("GameInstance: Loaded player data for PlayerID %d"), PlayerID);
		return *Found;
	}

	UE_LOG(LogTemp, Warning, TEXT("GameInstance: No saved data for PlayerID %d"), PlayerID);
	return FPlayerPersistentData();
}

bool UTeam9GameInstance::HasPlayerData(int32 PlayerID) const
{
	return PlayerDataMap.Contains(PlayerID);
}

void UTeam9GameInstance::ClearPlayerData(int32 PlayerID)
{
	PlayerDataMap.Remove(PlayerID);
}

void UTeam9GameInstance::ClearAllPlayerData()
{
	PlayerDataMap.Empty();
}
