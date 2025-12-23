// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileComponent.generated.h"

class UTileAction;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM9_PROJECT_API UTileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileComponent();

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TSubclassOf<UTileAction>> _OnPlayerArriveTileActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TSubclassOf<UTileAction>> _OnPlayerRollDiceTileActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TSubclassOf<UTileAction>> _OnPlayerLeaveTileActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TSubclassOf<UTileAction>> _OnPlayerUseItemTileActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TSubclassOf<UTileAction>> _OnPlayerPassedTileActions;

	UPROPERTY()
	TArray<TObjectPtr<UTileAction>> _TileActionInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;

	virtual void OnUnregister() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
private:
	void SetOnPlayerArriveTileActions();
	void SetOnPlayerRollDiceTileActions();
	void SetOnPlayerLeaveTileActions();
	void SetOnPlayerUseItemTileActions();
	void SetOnPlayerPassedTileActions();
};
