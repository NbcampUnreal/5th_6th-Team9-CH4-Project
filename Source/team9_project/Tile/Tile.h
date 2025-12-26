// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UStaticMeshComponent;
class UTileDataAsset;
class APlayerCharacter;
class UTileComponent;
struct FTileData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerArrive, APlayerCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerRollDice, APlayerCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerLeave, APlayerCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUseItem, APlayerCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerPassed, APlayerCharacter*);
//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRollDiceResult, APlayerCharacter*, int32);

UCLASS()
class TEAM9_PROJECT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	FOnPlayerArrive OnPlayerArrive;
	FOnPlayerLeave OnPlayerLeave;
	FOnPlayerUseItem OnPlayerUseItem;
	FOnPlayerRollDice OnPlayerRollDice;
	FOnPlayerPassed OnPlayerPassed;

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TObjectPtr<UStaticMeshComponent> _Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TObjectPtr<USceneComponent> _Root;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TWeakObjectPtr<ATile>> _BeforeTile;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TWeakObjectPtr<ATile>> _NextTile;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Tile")
	TArray<TObjectPtr<UTileComponent>> _TileComponents;

	int32 _Index;

	TSet<TWeakObjectPtr<APlayerCharacter>> _InPlayers;

public:
	void AssignFromData(FTileData* data, int32 index);
	void SetLinkTiles(const TArray<TWeakObjectPtr<ATile>>& BeforeTile, const TArray<TWeakObjectPtr<ATile>>& NextTile);

	void PlayerArrive(APlayerCharacter* PlayerCharacter);
	void PlayerPassed(APlayerCharacter* PlayerCharacter);
	void PlayerLeave(APlayerCharacter* PlayerCharacter);
	void PlayerUseItem(APlayerCharacter* PlayerCharacter);
	void PlayerRollDice(APlayerCharacter* PlayerCharacter);

	TArray<APlayerCharacter*> GetInPlayers();

	TArray<ATile*> GetNextTiles();
	TArray<ATile*> GetBeforeTiles();

	int32 GetIndex() {
		return _Index;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AssignFromDataAsset(UTileDataAsset* asset);

	bool IsServer();
};
