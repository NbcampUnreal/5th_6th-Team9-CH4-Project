#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();



	virtual void CopyProperties(APlayerState* NewPlayerState) override;


	void SetHP(int Amount);

	int GetHP();

	void SetMaxHP(int Amount);

	int GetMaxHP();

	void AddHP(int Amount);

	void SetScore(int Amount);

	int GetScore();

	void AddScore(int Amount);

	void SetTileIndex(int Amount);

	int GetTileIndex();

	int32 GetPlayerNumber();

	void SetPlayerNumber(int32 PNumber);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	int CurrentHp;

	int MaxHp;

	//int CurrentScore;

	int TileIndex;

	//int32 PlayerNumber;

public:
	    // UI용 표시 이름 (복제로 모든 클라이언트에 전달됨)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    FString DisplayName;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    bool bIsReady = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    int32 PlayerNumber = -1;

    // 점수와 순위 변수 (복제 설정)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Result")
    int32 CurrentScore = 0;
	
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Result")
    int32 FinalRank = 0;

};
