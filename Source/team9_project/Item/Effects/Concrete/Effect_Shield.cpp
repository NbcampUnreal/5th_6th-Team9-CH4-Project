// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Shield.h"
#include "Item/Data/ItemUseContext.h"
#include "Inventory/InventoryComponent.h"

void UEffect_Shield::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Shield: User is null"));
		return;
	}

	// 인벤토리 컴포넌트 가져오기
	UInventoryComponent* InventoryComp = User->FindComponentByClass<UInventoryComponent>();
	if (!InventoryComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Shield: InventoryComponent not found on User"));
		return;
	}

	// 실드 효과 생성
	FActiveEffect ShieldEffect;
	ShieldEffect.EffectID = FName("Shield");
	ShieldEffect.EndCoudition = EEffectEndCondition::CountBased;
	ShieldEffect.RemainingRounds = 0; // CountBased는 라운드 사용 안 함
	ShieldEffect.RemainingCount = ShieldCount;

	// 이미 실드가 있으면 횟수 누적
	if (InventoryComp->HasEffect(FName("Shield")))
	{
		// 기존 실드 제거
		InventoryComp->RemoveEffect(FName("Shield"));
	}

	// 새 실드 추가
	InventoryComp->AddEffect(ShieldEffect);

	UE_LOG(LogTemp, Log, TEXT("Effect_Shield: Shield effect applied to %s"), *User->GetName());
}
