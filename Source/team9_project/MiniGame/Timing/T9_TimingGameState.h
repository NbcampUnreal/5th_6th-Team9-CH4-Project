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

    FText GameName = FText::FromString(TEXT("TimingGame"));

    FText GameDescription = FText::FromString(TEXT("폭탄 게임"));

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UTexture2D> GameTexture;
};
