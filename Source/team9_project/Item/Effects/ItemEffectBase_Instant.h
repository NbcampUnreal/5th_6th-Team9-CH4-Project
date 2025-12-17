// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase.h"
#include "ItemEffectBase_Instant.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable)
class TEAM9_PROJECT_API UItemEffectBase_Instant : public UItemEffectBase
{
	GENERATED_BODY()
	
	public:
	
	virtual void StartUse(AActor* User) override;
	
	virtual EItemUseType GetUseType() const override;
	
};
