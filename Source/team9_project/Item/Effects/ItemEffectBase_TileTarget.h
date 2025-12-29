// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase.h"
#include "ItemEffectBase_TileTarget.generated.h"

class ATileManagerActor;
class APlayerCharacter;
class ATileSelectorActor;

/**
 * Cho_SungMin - 타일 타겟 아이템 효과 기본 클래스
 * A/D 키로 타일을 순환 선택하고, Space로 확정하는 방식
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
	virtual void UpdateContext(FItemUseContext& Context) override;

	//타일 선택
	UFUNCTION(BlueprintCallable)
	void CycleNextTile();

	UFUNCTION(BlueprintCallable)
	void CyclePrevTile();

	UFUNCTION(BlueprintCallable)
	int32 GetSelectedTileIndex() const;

	// Cho_SungMin - 카메라 추적용 타일 셀렉터 위치 반환
	UFUNCTION(BlueprintCallable)
	FVector GetTileSelectorLocation() const;

protected:

	UPROPERTY(blueprintReadOnly)
	int32 SelectedTileIndex = 0;

	UPROPERTY(blueprintReadOnly)
	int32 TotalTileCount = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float TimeLimit = 30.0f;

	// Cho_SungMin - 타일 선택 시각화 액터
	UPROPERTY()
	TObjectPtr<ATileSelectorActor> TileSelectorActor;

	UPROPERTY(EditDefaultsOnly, Category = "TileSelector")
	TSubclassOf<ATileSelectorActor> TileSelectorClass;

	// Cho_SungMin - 셀렉터 액터 생성/파괴
	void SpawnTileSelector();
	void DestroyTileSelector();
	void UpdateTileSelectorPosition();

	//타일 목록 초기화
	virtual void InitializeTileList();

	bool CheckTimeout();

	// Cho_SungMin - 타일 선택 입력 쿨다운
	float TileCycleCooldown = 0.3f;
	float LastTileCycleTime = 0.0f;

};
