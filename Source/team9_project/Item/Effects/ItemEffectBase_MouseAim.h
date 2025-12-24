// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Effects/ItemEffectBase.h"
#include "ItemEffectBase_MouseAim.generated.h"

/**
 *
 */
UCLASS(Abstract,Blueprintable)
class TEAM9_PROJECT_API UItemEffectBase_MouseAim : public UItemEffectBase
{
	GENERATED_BODY()

public:
	virtual void StartUse(AActor* User) override;
	virtual void TickUse(float DeltaTime) override;
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;
	virtual void CancelUse() override;
	virtual EItemUseType GetUseType() const override;
	virtual void UpdateContext(FItemUseContext& Context) override;

	//마우스 조준 방향 가져오기
	UFUNCTION(BlueprintCallable)
	FVector GetAimDirection() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector AimDirection = FVector::ForwardVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeLimt = 30.0f;

	virtual void UpdateAimDirection();

	bool ChackTimeout();

};
