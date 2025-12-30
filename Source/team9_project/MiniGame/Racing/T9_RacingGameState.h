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

    FText GameName = FText::FromString(TEXT("RacingGame"));

    FText GameDescription = FText::FromString(TEXT("RacingGameDescription"));

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UTexture2D> GameTexture;
};
