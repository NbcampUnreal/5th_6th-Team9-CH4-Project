// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlots();
	
}

void UInventoryComponent::InitializeSlots()
{
	Slots.Empty();
	Slots.SetNum(Max_SLOTS);
    
	for (int32 i = 0; i < Max_SLOTS; ++i)
	{
		Slots[i].Clear();
	}
}
void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, Slots);
	DOREPLIFETIME(UInventoryComponent, ActiveEffects);
	DOREPLIFETIME(UInventoryComponent, bHasUsedItemThisTurn);
}



void UInventoryComponent::OnRep_ActiveEffects()
{
	OnEffectUpdated.Broadcast(NAME_None);
}

void UInventoryComponent::OnRep_Slots()
{
	OnInventoryUpdated.Broadcast(-1);
}


bool UInventoryComponent::AddItem(FName ItemID)
{
	int32 EmptySlot = GetEmptySlotIndex();
	if (EmptySlot == -1)
	{
		return false;
	}
	
	Slots[EmptySlot].SetItem(ItemID);
	OnInventoryUpdated.Broadcast(EmptySlot);
	return true;
}

bool UInventoryComponent::RemoveItem(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= Max_SLOTS)
	{
		return false;
	}
	if (Slots[SlotIndex].IsEmpty())
	{
		return false;
	}
	Slots[SlotIndex].Clear();
	OnInventoryUpdated.Broadcast(SlotIndex);
	return true;
	
}


bool UInventoryComponent::SwapSlots(int32 FromIndex, int32 ToIndex)
{
	if (FromIndex < 0 || FromIndex >= Max_SLOTS || ToIndex < 0 || ToIndex >= Max_SLOTS)
	{
		return false;
	}
	
	FInventorySlot TempSlot = Slots[FromIndex];
	Slots[FromIndex] = Slots[ToIndex];
	Slots[ToIndex] = TempSlot;
	return true;
}

bool UInventoryComponent::UseItem(int32 SlotIndex)
{
	if (!CanUseItem())
	{
		return false;
	}
	
	if (SlotIndex < 0 || SlotIndex >= Max_SLOTS)
	{
		return false;
	}
	
	if (Slots[SlotIndex].IsEmpty())
	{
		return false;
	}
	
	bHasUsedItemThisTurn  = true;
	return true;
	
	
}

bool UInventoryComponent::CanUseItem() const
{
	return !bHasUsedItemThisTurn;
}

FInventorySlot UInventoryComponent::GetSlot(int32 SlotIndex) const
{
	if (SlotIndex < 0 || SlotIndex >= Max_SLOTS)
	{
		return FInventorySlot();
	}
	return Slots[SlotIndex];
}

bool UInventoryComponent::IsSlotEmpty(int32 SlotIndex) const
{
	if (SlotIndex < 0 || SlotIndex >= Max_SLOTS)
	{
		return true;
	}
	return Slots[SlotIndex].IsEmpty();
}

int32 UInventoryComponent::GetEmptySlotIndex() const
{
	for (int32 i = 0; i < Max_SLOTS; ++i)
	{
		if (Slots[i].IsEmpty())
		{
			return i;
		}
	}
	return -1;
}

bool UInventoryComponent::IsInventoryFull() const
{
	return GetEmptySlotIndex() == -1;
}

void UInventoryComponent::AddEffect(const FActiveEffect& Effect)
{
	for (FActiveEffect& Existing : ActiveEffects)
	{
		if (Existing.EffectID ==  Effect.EffectID)
		{
			Existing.RemainingRounds += Effect.RemainingRounds;
			Existing.RemainingRounds += Effect.RemainingCount;
			OnEffectUpdated.Broadcast(Existing.EffectID);
			return;
		}
	}
	
	ActiveEffects.Add(Effect);
	OnEffectUpdated.Broadcast(Effect.EffectID);
}

void UInventoryComponent::RemoveEffect(FName EffectID)
{
	ActiveEffects.RemoveAll([EffectID](const FActiveEffect& Effect)
	{
		return Effect.EffectID == EffectID;
	});
	
		OnEffectUpdated.Broadcast(EffectID);
	
}

bool UInventoryComponent::HasEffect(FName EffectID) const
{
	for (const FActiveEffect& Effect : ActiveEffects)
	{
		if (Effect.EffectID == EffectID)
		{
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::TryConsumeEffect(FName EffectID)
{
	for (int32 i = 0; i < ActiveEffects.Num(); i++)
	{
		if (ActiveEffects[i].EffectID == EffectID)
		{
			if (ActiveEffects[i].EndCoudition == EEffectEndCondition::CountBased)
			{
				ActiveEffects[i].RemainingCount--;
				if (ActiveEffects[i].RemainingCount <= 0)
				{
					ActiveEffects.RemoveAt(i);
				}
				OnEffectUpdated.Broadcast(EffectID);
				return true;
			}
		}
	}
	return false;
}

void UInventoryComponent::OnRoundEnd()
{
	for (int32 i = ActiveEffects.Num() - 1; i >= 0; i--)
	{
		if (ActiveEffects[i].EndCoudition == EEffectEndCondition::RoundBased)
		{
			ActiveEffects[i].RemainingRounds--;
			if (ActiveEffects[i].RemainingRounds <= 0)
			{
				FName EffectID = ActiveEffects[i].EffectID;
				ActiveEffects.RemoveAt(i);
				OnEffectUpdated.Broadcast(EffectID);
			}
		}
	}
}

void UInventoryComponent::ResetTurnState()
{
	bHasUsedItemThisTurn = false;
}









