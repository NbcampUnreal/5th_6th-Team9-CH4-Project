// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffectBase.h"
#include "ItemEffectBase_DirectControl.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable)
class TEAM9_PROJECT_API UItemEffectBase_DirectControl : public UItemEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void StartUse(AActor* User) override;
	virtual void TickUse(float DeltaTime) override;
	virtual void ExecuteEffect(AActor* User, const FItemUseContext& Context) override;
	virtual void CancelUse() override;
	virtual EItemUseType GetUseType() const override;
	
	//이동 입력
	UFUNCTION(BlueprintCallable)
	void SetMoveInput(FVector2D Input);
	
	UFUNCTION(BlueprintCallable)
	AActor* GetControlledActor() const;
	
protected:
	 //조작 대상 액터
	UPROPERTY(BlueprintReadOnly)
	AActor* ControlledActor = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	FVector CurrentLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D MoveInput = FVector2D::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float MoveSpeed = 500.0f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float TimeLimit  = 30.0f;
	
	virtual AActor* SpawnControlledActor(AActor* User);
	
	virtual void DestroyControlledActor();
	
	virtual void UpdateMovement(float DeltaTime);
	
	bool CheckTimeout();
};
