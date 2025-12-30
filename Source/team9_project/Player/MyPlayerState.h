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

	UFUNCTION(Server, Reliable)
	void ServerRPC_CallMainGameStart();

private:
	UPROPERTY(Replicated)
	int CurrentHp;

	UPROPERTY(Replicated)
	int MaxHp;

	//int CurrentScore;

	// Cho_SungMin - TileIndex를 Replicated로 변경 (텔레포트 후 이동 버그 수정)
	UPROPERTY(Replicated)
	int TileIndex;

	//int32 PlayerNumber;

public:
	    // UI�� ǥ�� �̸� (������ ��� Ŭ���̾�Ʈ�� ���޵�)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    FString DisplayName;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    bool bIsReady = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    int32 PlayerNumber = -1;

    // ������ ���� ���� (���� ����)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Result")
    int32 CurrentScore = 0;
	
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Result")
    int32 FinalRank = 0;

};
