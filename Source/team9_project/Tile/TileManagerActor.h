// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManagerActor.generated.h"

class ATile;
class APlayerCharacter;
struct FTileData;

UCLASS()
class TEAM9_PROJECT_API ATileManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileManagerActor();

	static ATileManagerActor* Get(UWorld* World)
	{
		if (SingletonInstance.IsValid()) return SingletonInstance.Get();
		return nullptr;
	}

	ATile* GetTile(int32 Index);

protected:


private:
	static TWeakObjectPtr<ATileManagerActor> SingletonInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> _TilesData;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ATile>> _Tiles;

public:	
	//void PlayerArrive();
	void PlayerArrive(int32 TileIndex, APlayerCharacter* PlayerCharacter);
	void PlayerPassed(int32 TileIndex, APlayerCharacter* PlayerCharacter);
	void PlayerLeave(int32 TileIndex, APlayerCharacter* PlayerCharacter);
	void PlayerUseItem(int32 TileIndex, APlayerCharacter* PlayerCharacter);
	void PlayerRollDice(int32 TileIndex, APlayerCharacter* PlayerCharacter);

protected:
	virtual void BeginPlay() override;

private:	
	void SpawnTiles();
	void LinkTiles();
	TArray<TWeakObjectPtr<ATile>> GetTilesByIndexes(TArray<int32>& Indexes) const ;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerArrive(ATile* Tile, APlayerCharacter* PlayerCharacter);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerPassed(ATile* Tile, APlayerCharacter* PlayerCharacter);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerLeave(ATile* Tile, APlayerCharacter* PlayerCharacter);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerUseItem(ATile* Tile, APlayerCharacter* PlayerCharacter);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerRollDice(ATile* Tile, APlayerCharacter* PlayerCharacter);
};
