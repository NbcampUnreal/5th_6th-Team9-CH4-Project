// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Data/ItemData.h"
#include "Item/Data/ItemUseContext.h"
#include "InventoryComponent.generated.h"

class UItemSubsystem;
class UItemEffectBase;
class AAimIndicatorActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemUseStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemUseCancelled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectUpdated, FName, EffectID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUseResult, FName, ItemID, bool, bSuccess);

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
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsCurrentlyOperating = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	AActor* CurrentControlledActor = nullptr;
	//델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnEffectUpdated OnEffectUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnItemUseResult OnItemUseResult;

	UPROPERTY(BlueprintAssignable)
	FOnItemUseStarted OnItemUseStarted;

	UPROPERTY(BlueprintAssignable)
	FOnItemUseCancelled OnItemUseCancelled;
	
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetMouseAimDirection(FVector Direction);

	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector CurrentAimDirection = FVector::ZeroVector;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	AAimIndicatorActor* CurrentAimIndicator = nullptr;

	// AimIndicator 클래스 (BP 설정용)
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AAimIndicatorActor> AimIndicatorClass;
	//========== 네트워크 RPC ==========

	// 클라이언트 → 서버: 아이템 사용 요청
	UFUNCTION(Server, Reliable, WithValidation,BlueprintCallable)
	void Server_RequestUseItem(int32 SlotIndex, FItemUseContext Context);

	// 서버 → 모든 클라이언트: 아이템 사용 결과 브로드캐스트
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnItemUsed(FName ItemID, bool bSuccess);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_DrawDebugAttack(FVector Center, float Radius, FVector Start, FVector End);
	
	//아이템 관리
	UFUNCTION(BlueprintCallable)
	bool AddItem(FName ItemID);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	bool SwapSlots(int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintCallable)
	bool UseItem(int32 SlotIndex);

	// 아이템 사용 확정 (조작형 아이템용)
	UFUNCTION(BlueprintCallable)
	void ConfirmItemUse();

	// 아이템 사용 취소
	UFUNCTION(BlueprintCallable)
	void CancelItemUse();

	// 현재 아이템 사용 중인지 확인
	UFUNCTION(BlueprintCallable)
	bool IsUsingItem() const;

	// 현재 사용 중인 Effect 가져오기
	UFUNCTION(BlueprintCallable)
	UItemEffectBase* GetCurrentEffect() const;

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

	// Cho_Sungmin - GameInstance 연동 (맵 이동 시 데이터 유지)
	UFUNCTION(BlueprintCallable, Category = "Persistence")
	void SaveToGameInstance(int32 PlayerID);

	UFUNCTION(BlueprintCallable, Category = "Persistence")
	void LoadFromGameInstance(int32 PlayerID);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetDirectControlInput(FVector2D Input);
	
	
protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveEffects();

private:
	void InitializeSlots();

	// ItemSubsystem 가져오기
	UItemSubsystem* GetItemSubsystem() const;

	// 현재 사용 중인 Effect
	UPROPERTY()
	UItemEffectBase* CurrentEffect = nullptr;

	// 현재 사용 중인 아이템 ID
	UPROPERTY()
	FName CurrentItemID = NAME_None;

	// 현재 사용 중인 아이템의 Context
	FItemUseContext CurrentUseContext;

	// Effect 정리
	void ClearCurrentEffect();

	// Effect가 자체적으로 취소되었을 때 처리 (타임아웃 등)
	void HandleEffectCancelled();
	
	void SpawnAimIndicator();
	void DestroyAimIndicator();

};
