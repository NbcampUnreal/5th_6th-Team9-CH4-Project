// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffectBase.h"
#include "Item/Data/ItemUseContext.h"

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
}

EItemUseType UItemEffectBase::GetUseType() const
{
	return EItemUseType::Instant;
}