// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "item/data/ItemTypes.h"
#include "ItemEffectBase.generated.h"

struct FItemUseContext;

UCLASS(abstract, Blueprintable)
class TEAM9_PROJECT_API UItemEffectBase : public UObject
{
	GENERATED_BODY()
	
	public:
	UFUNCTION(BlueprintCallable)
	virtual void StartUse(AActor* User);
	
	UFUNCTION(BlueprintCallable)
	virtual void TickUse(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context);
	
	UFUNCTION(BlueprintCallable)
	virtual void CancelUse();
	
	UFUNCTION(BlueprintCallable)
	virtual EItemUseType GetUseType() const;
	
	protected:
	UPROPERTY()
	AActor* CurrentUser = nullptr;
	
	UPROPERTY()
	float ElapsedTime = 0.0f;
	
	UPROPERTY()
	bool bIsOperating = false;
	
};
