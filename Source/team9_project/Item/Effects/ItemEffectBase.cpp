// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffectBase.h"

void UItemEffectBase::StartUse(AActor* User)
{
	CurrentUser = User;
	ElapsedTime = 0.0f;
	bIsOperating = true;
}

void UItemEffectBase::TickUse(float DeltaTime)
{
	if (bIsOperating)
	{
		ElapsedTime += DeltaTime;
	}
}

void UItemEffectBase::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	bIsOperating = false;
}

void UItemEffectBase::CancelUse()
{
	bIsOperating = false;
	CurrentUser = nullptr;
	ElapsedTime = 0.0f;

	// 취소 델리게이트 브로드캐스트
	OnEffectCancelled.Broadcast();
}

EItemUseType UItemEffectBase::GetUseType() const
{
	return EItemUseType::Instant;
}

void UItemEffectBase::UpdateContext(FItemUseContext& Context)
{
	// 기본 구현: 아무것도 하지 않음
	// 서브클래스에서 오버라이드하여 자신의 값을 Context에 설정
}
