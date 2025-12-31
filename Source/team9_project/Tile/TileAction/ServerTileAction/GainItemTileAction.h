// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/TileAction/ServerTileAction/ServerTileAction.h"
#include "GainItemTileAction.generated.h"

UCLASS()
class TEAM9_PROJECT_API UGainItemTileAction : public UServerTileAction
{
	GENERATED_BODY()

public:
	virtual void Active(APlayerCharacter* PlayerCharacter) override;

private:
	// 획득할 아이템 개수 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "5"))
	int32 ItemCount = 1;

	// 특정 아이템만 드롭하도록 지정 (빈 값이면 랜덤)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	FName SpecificItemID = NAME_None;
};
