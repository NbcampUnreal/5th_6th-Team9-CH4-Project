// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UStaticMeshComponent;
class UTileDataAsset;
struct FTileData;

UCLASS()
class TEAM9_PROJECT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

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

	int32 Index;

public:
	void AssignFromData(FTileData* data, int32 index);
	void SetLinkTiles(const TArray<TWeakObjectPtr<ATile>>& BeforeTile, const TArray<TWeakObjectPtr<ATile>>& NextTile);
	//void SetConectedTiles(FTileData& data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AssignFromDataAsset(UTileDataAsset* asset);
};
