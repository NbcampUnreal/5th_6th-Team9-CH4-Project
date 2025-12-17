// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Data/ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectUpdated, FName, EffectID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM9_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	static constexpr int32 Max_SLOTS = 5;
	
	//인벤토리 슬롯
	UPROPERTY(ReplicatedUsing=OnRep_Slots, BlueprintReadOnly)
	TArray<FInventorySlot> Slots;
	
	//활성 효과
	UPROPERTY(ReplicatedUsing=OnRep_ActiveEffects, BlueprintReadOnly)
	TArray<FActiveEffect> ActiveEffects;
	
	//아이템 사용 여부
	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bHasUsedItemThisTurn = false;
	
	//델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FOnEffectUpdated OnEffectUpdated;
	
	//아이템 관리
	UFUNCTION(BlueprintCallable)
	bool AddItem(FName ItemID);
	
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	bool SwapSlots(int32 FromIndex, int32 ToIndex);
	
	UFUNCTION(BlueprintCallable)
	bool UseItem(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	bool CanUseItem() const;
	
	UFUNCTION(BlueprintCallable)
	FInventorySlot GetSlot(int32 SlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	bool IsSlotEmpty(int32 SlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetEmptySlotIndex() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsInventoryFull() const;
	
	//효과 관리
	UFUNCTION(BlueprintCallable)
	void AddEffect(const FActiveEffect& Effect);
	
	UFUNCTION(BlueprintCallable)
	void RemoveEffect(FName EffectID);
	
	UFUNCTION(BlueprintCallable)
	bool HasEffect(FName EffectID) const;
	
	UFUNCTION(BlueprintCallable)
	bool TryConsumeEffect(FName EffectID);
	
	//턴 리셋
	UFUNCTION(BlueprintCallable)
	void OnRoundEnd();
	
	UFUNCTION(BlueprintCallable)
	void ResetTurnState();

	
protected:
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_Slots();
	
	UFUNCTION()
	void OnRep_ActiveEffects();

private:
	void InitializeSlots();
	

		
};
