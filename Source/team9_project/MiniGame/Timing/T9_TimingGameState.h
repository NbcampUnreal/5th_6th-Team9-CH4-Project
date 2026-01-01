#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "T9_TimingGameState.generated.h"

USTRUCT(BlueprintType)
struct FTimingPlayerResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 PlayerId;

    UPROPERTY(BlueprintReadOnly)
    int32 PressTimeMs;

    UPROPERTY(BlueprintReadOnly)
    int32 DeltaMs;
};

UCLASS()
class TEAM9_PROJECT_API AT9_TimingGameState : public AT9_MiniGameStateBase
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void OnRep_Results();

	virtual void OnPhaseChanged(EMiniGamePhase NewPhase) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void AddPlayerPress(int32 PlayerId, float PressServerTime);

    virtual FText GetGameName() override;

    virtual FText GetGameDescription() override;

    virtual UTexture2D* GetGameTexture() override;

public:
    UPROPERTY(Replicated)
    float GameStartTime;

    UPROPERTY(Replicated)
    int32 TargetTimeMs;

    UPROPERTY(ReplicatedUsing = OnRep_Results, BlueprintReadOnly)
    TArray<FTimingPlayerResult> Results;

    FText GameName = FText::FromString(TEXT("BOMB ZERO"));

    FText GameDescription =
        FText::FromString(TEXT("게임 설명\n전장에 설치된 폭탄이 폭발하기 직전,\n정확한 타이밍에 버튼을 눌러 폭탄을 무력화하라.\n 목표 시간과의 오차가 가장 작은 플레이어가 승리한다.\n\n\n 승리 조건\n 5초 뒤 사라지는 타이머가 0이되어 \n터지기 전에 STOP버튼을 최대한 늦게 누르는 플레이어가 승리"));

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UTexture2D> GameTexture;
};
