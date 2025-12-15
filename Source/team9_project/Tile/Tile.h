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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> _Mesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<ATile>> _BeforeTile;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<ATile>> _AffterTile;

	int32 Index;

public:
	void AssignFromData(FTileData* data);
	//void SetConectedTiles(FTileData& data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AssignFromDataAsset(UTileDataAsset* asset);

};
