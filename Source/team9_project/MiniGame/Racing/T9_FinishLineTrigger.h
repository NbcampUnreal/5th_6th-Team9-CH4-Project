#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T9_FinishLineTrigger.generated.h"

class UBoxComponent;

UCLASS()
class TEAM9_PROJECT_API AT9_FinishLineTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AT9_FinishLineTrigger();

	UFUNCTION()
	void OnFinishOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FinishLine")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FinishLine")
	UBoxComponent* FinishTrigger;


};
