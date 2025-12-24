#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "T9_MiniGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EMiniGamePhase : uint8
{
	None,
	Ready,
	Playing,
	Result
};

USTRUCT(BlueprintType)
struct FMiniGamePlayerResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerId = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 Score = 0;
};

USTRUCT(BlueprintType)
struct FMiniGamePlayerReady
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerId = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bReady = false;
};

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnRep_Phase();

	UFUNCTION()
	void OnRep_PlayerReady();

	UFUNCTION()
	void InitializePlayerReadys();

	UFUNCTION()
	void SetPlayerReady(int32 PlayerNumber, bool bReady);

	void AddPlayerResult(int32 PlayerId, int32 Score);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnPhaseChanged(EMiniGamePhase NewPhase);

	void SetPhase(EMiniGamePhase NewPhase);


public:
	UPROPERTY(Replicated)
	FMiniGamePlayerResult PlayerResults;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerReady)
	TArray<FMiniGamePlayerReady> PlayerReadys;

	bool bInGame = false;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Phase)
	EMiniGamePhase CurrentPhase;
};
