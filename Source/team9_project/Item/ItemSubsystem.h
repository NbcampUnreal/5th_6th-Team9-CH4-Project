// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Item/Data/ItemData.h"
#include "ItemSubsystem.generated.h"

class UDataTable;
class UItemEffectBase;

/**
 * 아이템 서브시스템 - 아이템 데이터 관리
 * GameInstance에 종속되어 레벨 전환에도 유지됨
 */
UCLASS()
class TEAM9_PROJECT_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 서브시스템 초기화
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// 아이템 데이터 조회
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetItemData(FName ItemID) const;

	// 아이템 데이터가 존재하는지 확인
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool HasItemData(FName ItemID) const;

	// 아이템 효과 클래스 생성
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemEffectBase* CreateItemEffect(FName ItemID, UObject* Outer);

	// 모든 아이템 ID 목록 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item")
	TArray<FName> GetAllItemIDs() const;

	// 랜덤 아이템 ID 가져오기 (드롭 가중치 적용)
	UFUNCTION(BlueprintCallable, Category = "Item")
	FName GetRandomItemID() const;

	// DataTable 설정 (블루프린트에서 설정 가능)
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemDataTable(UDataTable* InDataTable);

protected:
	// 아이템 DataTable
	UPROPERTY()
	UDataTable* ItemDataTable;

	// 캐싱된 아이템 데이터 (빠른 조회용)
	UPROPERTY()
	TMap<FName, FItemData> CachedItemData;

	// DataTable에서 데이터 캐싱
	void CacheItemData();
};
