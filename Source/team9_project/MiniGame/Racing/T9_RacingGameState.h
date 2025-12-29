#pragma once

#include "CoreMinimal.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "T9_RacingGameState.generated.h"

USTRUCT()
struct FRacingPlayerDistance : public FFastArraySerializerItem
{
    GENERATED_BODY()

    UPROPERTY()
    int32 PlayerId;

    UPROPERTY()
    int32 Distance;

    void PostReplicatedAdd(const struct FRacingPlayerDistanceArray& InArray);

    void PostReplicatedChange(const struct FRacingPlayerDistanceArray& InArray);

    void PreReplicatedRemove(const struct FRacingPlayerDistanceArray& InArray);
};

USTRUCT()
struct FRacingPlayerDistanceArray : public FFastArraySerializer
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FRacingPlayerDistance> Items;

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<
            FRacingPlayerDistance,
            FRacingPlayerDistanceArray>(Items, DeltaParams, *this);
    }
};

template<>
struct TStructOpsTypeTraits<FRacingPlayerDistanceArray>
    : public TStructOpsTypeTraitsBase2<FRacingPlayerDistanceArray>
{
    enum { WithNetDeltaSerializer = true };
};

UCLASS()
class TEAM9_PROJECT_API AT9_RacingGameState : public AT9_MiniGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPhaseChanged(EMiniGamePhase NewPhase) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void UpdatePlayerDistance(int32 PlayerId, int32 Distance);

public:
    UPROPERTY(Replicated)
    FRacingPlayerDistanceArray PlayerDistances;
};
