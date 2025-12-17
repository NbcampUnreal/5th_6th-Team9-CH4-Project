// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase.h"
#include "ItemEffectBase_TileTarget.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable)
class TEAM9_PROJECT_API UItemEffectBase_TileTarget : public UItemEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void StartUse(AActor* User) override;
	virtual void TickUse(float DeltaTime) override;
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;
	virtual void CancelUse() override;
	virtual EItemUseType GetUseType() const override;
	
	//타일 선택
	UFUNCTION(blueprintCallable)
	void CycleNextTile();
	
	UFUNCTION(blueprintCallable)
	void CyclePrevTile();
	
	UFUNCTION(blueprintCallable)
	int32 GetSelectedTileIndex() const;
	
protected:
	
	UPROPERTY(blueprintReadOnly)
	int32 SelectedTileIndex = 0;
	
	UPROPERTY(blueprintReadOnly)
	int32 TotalTileCount = 0;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float TimeLimit = 30.0f;
	//타일 목록 초기화
	virtual void InitializeTileList();
	
	bool CheckTimeout();
	
};
