// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "item/data/ItemTypes.h"
#include "Item/Data/ItemUseContext.h"
#include "ItemEffectBase.generated.h"

// Effect 취소 델리게이트 (타임아웃 등)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEffectCancelled);

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

	// Context에 현재 Effect 값 업데이트 (서브클래스에서 오버라이드)
	UFUNCTION(BlueprintCallable)
	virtual void UpdateContext(UPARAM(ref) FItemUseContext& Context);

	// 현재 작동 중인지 확인
	UFUNCTION(BlueprintCallable)
	bool IsOperating() const { return bIsOperating; }

	// 취소 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnEffectCancelled OnEffectCancelled;

protected:
	UPROPERTY()
	AActor* CurrentUser = nullptr;

	UPROPERTY()
	float ElapsedTime = 0.0f;

	UPROPERTY()
	bool bIsOperating = false;

};
