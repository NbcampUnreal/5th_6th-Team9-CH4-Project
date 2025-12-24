// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ItemSubsystem.h"
#include "Item/Effects/ItemEffectBase.h"
#include "Engine/DataTable.h"

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// DataTable 자동 로드
	static const FString DataTablePath = TEXT("/Game/Item/Data/DT_ItemDataTable.DT_ItemDataTable");
	ItemDataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);

	if (ItemDataTable)
	{
		CacheItemData();
		UE_LOG(LogTemp, Log, TEXT("ItemSubsystem: DataTable loaded successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSubsystem: Failed to load DataTable at %s"), *DataTablePath);
	}

	UE_LOG(LogTemp, Log, TEXT("ItemSubsystem Initialized"));
}

void UItemSubsystem::Deinitialize()
{
	CachedItemData.Empty();
	ItemDataTable = nullptr;

	Super::Deinitialize();
}

void UItemSubsystem::SetItemDataTable(UDataTable* InDataTable)
{
	ItemDataTable = InDataTable;
	CacheItemData();
}

void UItemSubsystem::CacheItemData()
{
	CachedItemData.Empty();

	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSubsystem: ItemDataTable is null"));
		return;
	}

	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName, TEXT("ItemSubsystem"));
		if (ItemData)
		{
			CachedItemData.Add(ItemData->ItemID, *ItemData);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ItemSubsystem: Cached %d items"), CachedItemData.Num());
}

FItemData UItemSubsystem::GetItemData(FName ItemID) const
{
	if (const FItemData* Found = CachedItemData.Find(ItemID))
	{
		return *Found;
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemSubsystem: Item not found - %s"), *ItemID.ToString());
	return FItemData();
}

bool UItemSubsystem::HasItemData(FName ItemID) const
{
	return CachedItemData.Contains(ItemID);
}

UItemEffectBase* UItemSubsystem::CreateItemEffect(FName ItemID, UObject* Outer)
{
	const FItemData* ItemData = CachedItemData.Find(ItemID);
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSubsystem: Cannot create effect - Item not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	if (!ItemData->EffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSubsystem: EffectClass is null for item: %s"), *ItemID.ToString());
		return nullptr;
	}

	UItemEffectBase* Effect = NewObject<UItemEffectBase>(Outer, ItemData->EffectClass);
	return Effect;
}

TArray<FName> UItemSubsystem::GetAllItemIDs() const
{
	TArray<FName> ItemIDs;
	CachedItemData.GetKeys(ItemIDs);
	return ItemIDs;
}

FName UItemSubsystem::GetRandomItemID() const
{
	if (CachedItemData.Num() == 0)
	{
		return NAME_None;
	}

	// 가중치 합계 계산
	float TotalWeight = 0.0f;
	for (const auto& Pair : CachedItemData)
	{
		TotalWeight += Pair.Value.DropWeight;
	}

	// 랜덤 값 선택
	float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (const auto& Pair : CachedItemData)
	{
		CurrentWeight += Pair.Value.DropWeight;
		if (RandomValue <= CurrentWeight)
		{
			return Pair.Key;
		}
	}

	// 기본값 (첫 번째 아이템)
	TArray<FName> Keys;
	CachedItemData.GetKeys(Keys);
	return Keys.Num() > 0 ? Keys[0] : NAME_None;
}
