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
	void OnRep_Winner();

	UFUNCTION()
	void OnRep_PlayerReady();

	UFUNCTION()
	void InitializePlayerReadys();

	UFUNCTION()
	void SetPlayerReady(int32 PlayerNumber, bool bReady);

	void AddPlayerResult(int32 PlayerId, int32 Score);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnPhaseChanged(EMiniGamePhase NewPhase);

	virtual FText GetGameName();

	void SetPhase(EMiniGamePhase NewPhase);

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerReady)
	TArray<FMiniGamePlayerReady> PlayerReadys;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> GameImage;

	bool bInGame = false;

	UPROPERTY(ReplicatedUsing = OnRep_Winner)
	int32 WinnerPlayer = 0;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Phase)
	EMiniGamePhase CurrentPhase;
};
