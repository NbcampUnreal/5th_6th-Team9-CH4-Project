#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T9_RacingInGameWidget.generated.h"

USTRUCT(BlueprintType)
struct FRacingDistanceInfo
{
	GENERATED_BODY()

	int32 PlayerId;
	float Distance;
};

class AT9_FinishLineTrigger;

UCLASS()
class TEAM9_PROJECT_API UT9_RacingInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateRanking();

	void UpdateRankingUI(const TArray<FRacingDistanceInfo>& Distances);
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Ranking;

	UPROPERTY()

	TObjectPtr<AT9_FinishLineTrigger> CachedFinishLine;
};
