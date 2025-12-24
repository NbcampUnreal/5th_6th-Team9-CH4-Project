// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/Team9GameInstance.h"
#include "Item/ItemSubsystem.h"
#include "Item/Effects/ItemEffectBase.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlots();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 사용 중인 Effect가 있으면 TickUse 호출
	if (CurrentEffect)
	{
		CurrentEffect->TickUse(DeltaTime);

		// Effect가 타임아웃 등으로 스스로 종료되었는지 확인
		if (!CurrentEffect->IsOperating())
		{
			UE_LOG(LogTemp, Log, TEXT("TickComponent: Effect stopped operating, cancelling"));
			HandleEffectCancelled();
		}
	}
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

	// 이미 아이템 사용 중이면 실패
	if (IsUsingItem())
	{
		UE_LOG(LogTemp, Warning, TEXT("UseItem: Already using an item"));
		return false;
	}

	FName ItemID = Slots[SlotIndex].ItemID;

	// ItemSubsystem에서 Effect 생성
	UItemSubsystem* ItemSubsystem = GetItemSubsystem();
	if (!ItemSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseItem: ItemSubsystem not found"));
		return false;
	}

	UItemEffectBase* Effect = ItemSubsystem->CreateItemEffect(ItemID, this);
	if (!Effect)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseItem: Failed to create effect for %s"), *ItemID.ToString());
		return false;
	}

	// 슬롯에서 아이템 제거
	Slots[SlotIndex].Clear();
	OnInventoryUpdated.Broadcast(SlotIndex);

	// 아이템 사용 상태 설정
	bHasUsedItemThisTurn = true;
	CurrentItemID = ItemID;
	CurrentUseContext.OwnerActor = GetOwner();
	CurrentUseContext.OwnerInventory = this;

	// Effect 타입에 따라 분기
	EItemUseType UseType = Effect->GetUseType();

	if (UseType == EItemUseType::Instant)
	{
		// 즉시 발동: 바로 ExecuteEffect 호출
		Effect->ExecuteEffect(GetOwner(), CurrentUseContext);
		UE_LOG(LogTemp, Log, TEXT("UseItem: Instant effect executed for %s"), *ItemID.ToString());

		// 완료 브로드캐스트
		OnItemUseResult.Broadcast(ItemID, true);
	}
	else
	{
		// 조작형: StartUse 호출 후 대기
		CurrentEffect = Effect;
		CurrentEffect->StartUse(GetOwner());
		OnItemUseStarted.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("UseItem: Started using %s (Type: %d)"), *ItemID.ToString(), (int32)UseType);
	}

	return true;
}

bool UInventoryComponent::CanUseItem() const
{
	return !bHasUsedItemThisTurn && !IsUsingItem();
}

void UInventoryComponent::ConfirmItemUse()
{
	if (!CurrentEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfirmItemUse: No active effect"));
		return;
	}

	// Effect에서 최신 값으로 Context 업데이트
	CurrentEffect->UpdateContext(CurrentUseContext);

	// ExecuteEffect 호출
	CurrentEffect->ExecuteEffect(GetOwner(), CurrentUseContext);
	UE_LOG(LogTemp, Log, TEXT("ConfirmItemUse: Effect executed for %s"), *CurrentItemID.ToString());

	// 완료 브로드캐스트
	OnItemUseResult.Broadcast(CurrentItemID, true);

	// Effect 정리
	ClearCurrentEffect();
}

void UInventoryComponent::CancelItemUse()
{
	if (!CurrentEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("CancelItemUse: No active effect"));
		return;
	}

	// CancelUse 호출 (이미 취소된 경우에도 안전하게 호출)
	if (CurrentEffect->IsOperating())
	{
		CurrentEffect->CancelUse();
	}

	// 취소된 아이템 반환
	FName CancelledItemID = CurrentItemID;
	if (CancelledItemID != NAME_None)
	{
		AddItem(CancelledItemID);
		UE_LOG(LogTemp, Log, TEXT("CancelItemUse: Returned item %s to inventory"), *CancelledItemID.ToString());
	}

	// 취소 시 아이템 사용권 복구
	bHasUsedItemThisTurn = false;

	// 취소 브로드캐스트
	OnItemUseCancelled.Broadcast();
	OnItemUseResult.Broadcast(CancelledItemID, false);

	// Effect 정리
	ClearCurrentEffect();
}

void UInventoryComponent::HandleEffectCancelled()
{
	// Effect가 자체적으로 취소됨 (타임아웃 등)
	if (!CurrentEffect)
	{
		return;
	}

	// 취소된 아이템 반환
	FName CancelledItemID = CurrentItemID;
	if (CancelledItemID != NAME_None)
	{
		AddItem(CancelledItemID);
		UE_LOG(LogTemp, Log, TEXT("HandleEffectCancelled: Returned item %s to inventory"), *CancelledItemID.ToString());
	}

	// 아이템 사용권 복구
	bHasUsedItemThisTurn = false;

	// 취소 브로드캐스트
	OnItemUseCancelled.Broadcast();
	OnItemUseResult.Broadcast(CancelledItemID, false);

	// Effect 정리
	ClearCurrentEffect();
}

bool UInventoryComponent::IsUsingItem() const
{
	return CurrentEffect != nullptr;
}

UItemEffectBase* UInventoryComponent::GetCurrentEffect() const
{
	return CurrentEffect;
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

//========== 네트워크 RPC 구현 ==========

bool UInventoryComponent::Server_RequestUseItem_Validate(int32 SlotIndex, FItemUseContext Context)
{
	// 기본 유효성 검사
	if (SlotIndex < 0 || SlotIndex >= Max_SLOTS)
	{
		return false;
	}
	return true;
}

void UInventoryComponent::Server_RequestUseItem_Implementation(int32 SlotIndex, FItemUseContext Context)
{
	// 서버에서만 실행
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	// Context 저장 (클라이언트에서 전달받은 조준/타겟 정보)
	CurrentUseContext = Context;
	CurrentUseContext.OwnerActor = GetOwner();
	CurrentUseContext.OwnerInventory = this;

	// UseItem 호출 (로컬과 동일한 로직 사용)
	bool bSuccess = UseItem(SlotIndex);

	if (!bSuccess)
	{
		Multicast_OnItemUsed(NAME_None, false);
		return;
	}

	// Instant 타입이면 바로 Multicast
	// 조작형 타입이면 ConfirmItemUse 시점에 Multicast
	if (!IsUsingItem())
	{
		Multicast_OnItemUsed(CurrentItemID, true);
	}
}

void UInventoryComponent::Multicast_OnItemUsed_Implementation(FName ItemID, bool bSuccess)
{
	// 모든 클라이언트에서 실행
	OnItemUseResult.Broadcast(ItemID, bSuccess);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Multicast_OnItemUsed: Item %s used successfully"), *ItemID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Multicast_OnItemUsed: Item use failed"));
	}
}

//========== Cho_Sungmin - GameInstance 연동 ==========

void UInventoryComponent::SaveToGameInstance(int32 PlayerID)
{
	UTeam9GameInstance* GameInstance = Cast<UTeam9GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to get GameInstance for save"));
		return;
	}

	FPlayerPersistentData Data;
	Data.InventorySlots = Slots;
	Data.ActiveEffects = ActiveEffects;

	GameInstance->SavePlayerData(PlayerID, Data);
	UE_LOG(LogTemp, Log, TEXT("InventoryComponent: Saved inventory to GameInstance for PlayerID %d"), PlayerID);
}

void UInventoryComponent::LoadFromGameInstance(int32 PlayerID)
{
	UTeam9GameInstance* GameInstance = Cast<UTeam9GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to get GameInstance for load"));
		return;
	}

	if (!GameInstance->HasPlayerData(PlayerID))
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryComponent: No saved data for PlayerID %d, using defaults"), PlayerID);
		return;
	}

	FPlayerPersistentData Data = GameInstance->LoadPlayerData(PlayerID);

	// 인벤토리 슬롯 복원
	if (Data.InventorySlots.Num() > 0)
	{
		Slots = Data.InventorySlots;
		// 슬롯 수 맞추기
		while (Slots.Num() < Max_SLOTS)
		{
			FInventorySlot EmptySlot;
			EmptySlot.Clear();
			Slots.Add(EmptySlot);
		}
	}

	// 활성 효과 복원
	ActiveEffects = Data.ActiveEffects;

	// UI 업데이트
	OnInventoryUpdated.Broadcast(-1);
	OnEffectUpdated.Broadcast(NAME_None);

	UE_LOG(LogTemp, Log, TEXT("InventoryComponent: Loaded inventory from GameInstance for PlayerID %d"), PlayerID);
}

UItemSubsystem* UInventoryComponent::GetItemSubsystem() const
{
	if (!GetWorld())
	{
		return nullptr;
	}

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UItemSubsystem>();
}

void UInventoryComponent::ClearCurrentEffect()
{
	CurrentEffect = nullptr;
	CurrentItemID = NAME_None;
	CurrentUseContext = FItemUseContext();
}

