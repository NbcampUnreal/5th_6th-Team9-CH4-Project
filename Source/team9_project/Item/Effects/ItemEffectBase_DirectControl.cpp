// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffectBase_DirectControl.h"
#include "Item/Data/ItemUseContext.h"

void UItemEffectBase_DirectControl::StartUse(AActor* User)
{
	Super::StartUse(User);

	ControlledActor = SpawnControlledActor(User);

	if (ControlledActor)
	{
		CurrentLocation = ControlledActor->GetActorLocation();
	}
	else
	{
		CurrentLocation = User->GetActorLocation();

	}
	MoveInput = FVector2D::ZeroVector;
}

void UItemEffectBase_DirectControl::TickUse(float DeltaTime)
{
	Super::TickUse(DeltaTime);
	if (!bIsOperating)
	{
		return;
	}

	if (CheckTimeout())
	{
		CancelUse();
		return;
	}

	UpdateMovement(DeltaTime);

}

void UItemEffectBase_DirectControl::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);


}

void UItemEffectBase_DirectControl::CancelUse()
{
	Super::CancelUse();
	DestroyControlledActor();
	MoveInput = FVector2D::ZeroVector;
}

EItemUseType UItemEffectBase_DirectControl::GetUseType() const
{
	return EItemUseType::DirectControl;

}

void UItemEffectBase_DirectControl::SetMoveInput(FVector2D Input)
{
	MoveInput.X = FMath::Clamp(Input.X, -1.0f, 1.0f);
	MoveInput.Y = FMath::Clamp(Input.Y, -1.0f, 1.0f);
}

AActor* UItemEffectBase_DirectControl::GetControlledActor() const
{
	return ControlledActor;
}

void UItemEffectBase_DirectControl::UpdateContext(FItemUseContext& Context)
{
	Super::UpdateContext(Context);
	Context.FinalLocation = CurrentLocation;
}

AActor* UItemEffectBase_DirectControl::SpawnControlledActor(AActor* User)
{
	// 자식 클래스에서 구현
	// 예: RC카 스폰
	return nullptr;
}

void UItemEffectBase_DirectControl::DestroyControlledActor()
{
	if (ControlledActor)
	{
		ControlledActor->Destroy();
		ControlledActor = nullptr;
	}
}

void UItemEffectBase_DirectControl::UpdateMovement(float DeltaTime)
{
	if (!ControlledActor || MoveInput.IsNearlyZero())
	{
		return;
	}

	// MoveInput을 3D 벡터로 변환 (X=Forward, Y=Right)
	FVector MoveDelta = FVector(MoveInput.X, MoveInput.Y, 0.0f) * MoveSpeed * DeltaTime;

	// 현재 위치 업데이트
	CurrentLocation += MoveDelta;

	// 액터 위치 업데이트
	ControlledActor->SetActorLocation(CurrentLocation);
}

bool UItemEffectBase_DirectControl::CheckTimeout()
{
	return ElapsedTime >= TimeLimit;
}
