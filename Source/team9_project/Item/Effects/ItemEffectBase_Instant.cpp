// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Effects/ItemEffectBase_Instant.h"
#include "Item/Data/ItemUseContext.h"

void UItemEffectBase_Instant::StartUse(AActor* User)
{
	Super::StartUse(User);
	
	FItemUseContext Context;
	ExecuteEffect(User, Context);
}

EItemUseType UItemEffectBase_Instant::GetUseType() const
{
	return  EItemUseType::Instant;
}