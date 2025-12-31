#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "T9_RacingGameState.generated.h"

USTRUCT()
struct FRacingRanking
{
    GENERATED_BODY()

    UPROPERTY()
    int32 PlayerId;

    UPROPERTY()
    int32 Distance;
};

UCLASS()
class TEAM9_PROJECT_API AT9_RacingGameState : public AT9_MiniGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPhaseChanged(EMiniGamePhase NewPhase) override;

    virtual FText GetGameName() override;

    virtual FText GetGameDescription() override;

    virtual UTexture2D* GetGameTexture() override;

public:
    TArray<FRacingRanking> Ranking;

    FText GameName = FText::FromString(TEXT("SUPPLY RUN: FIRE LINE"));

    FText GameDescription = FText::FromString(TEXT("게임 설명\n전투 중단을 막기 위해 취사 지원을 수행하라.\n불안정하게 움직이는 화로를 조작해\n가장 먼저 결승 지점에 도달하는 플레이어가 승리한다.\n\n\n승리 조건\nz키를 연타로 눌러 화로를 가속 시킬 수 있으며 \n양쪽 방향키를 통해 장애물을 피하여 \n결승점에 먼저 도착한 플레이어가 승리"));

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UTexture2D> GameTexture;
};
