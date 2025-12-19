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

public:
    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 TargetTime;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float GameStartTime;

    UPROPERTY(ReplicatedUsing = OnRep_Results, BlueprintReadOnly)
    TArray<FTimingPlayerResult> Results;
};
