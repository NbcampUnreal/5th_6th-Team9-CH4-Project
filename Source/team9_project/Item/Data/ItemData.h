#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "ItemData.generated.h"

class UItemEffectBase;
class UTexture2D;

//아이템데이터
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropWeight = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OperationTimeLimit = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemEffectBase> EffectClass;
	
};

//인벤토리 슬롯
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEmpty = true;
	
	bool IsEmpty() const {return bIsEmpty;}
	
	void Clear()
	{
		ItemID = NAME_None;
		bIsEmpty = true;
	}
	void SetItem(FName NewItemID) 
	{ 
		ItemID = NewItemID; 
		bIsEmpty = false; 
	}
};


USTRUCT(BlueprintType)
struct FActiveEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EffectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectEndCondition EndCoudition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingRounds =0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingCount = 0;



};

// Cho_Sungmin - 맵 이동 시 유지되는 인벤토리 데이터
USTRUCT(BlueprintType)
struct FPlayerPersistentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActiveEffect> ActiveEffects;

	void Clear()
	{
		InventorySlots.Empty();
		ActiveEffects.Empty();
	}
};
