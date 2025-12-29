// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Effects/ItemEffectBase_MouseAim.h"
#include "Item/Data/ItemUseContext.h"
#include "GameFramework/PlayerController.h"

void UItemEffectBase_MouseAim::StartUse(AActor* User)
{
	Super::StartUse(User);
	bIsOperating = true;      
	CurrentUser = User;       
	ElapsedTime = 0.0f;       
	AimDirection = FVector::ForwardVector;

}
void UItemEffectBase_MouseAim::SetAimDirection(FVector Direction)
{
	AimDirection = Direction.GetSafeNormal();
}
void UItemEffectBase_MouseAim::TickUse(float DeltaTime)
{
	Super::TickUse(DeltaTime);

	if (!bIsOperating)
	{
		return;
	}
	if (ChackTimeout())
	{
		CancelUse();
		return;
	}

	UpdateAimDirection();
}

void UItemEffectBase_MouseAim::UpdateAimDirection()
{
	if (!CurrentUser)
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(CurrentUser);

	if (!Pawn)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (!PlayerController)
	{
		return;
	}

	FVector WorldLocation , WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		AimDirection = WorldDirection;
	}
}


void UItemEffectBase_MouseAim::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);



}

void UItemEffectBase_MouseAim::CancelUse()
{
	Super::CancelUse();
	AimDirection = FVector::ForwardVector;
}


EItemUseType UItemEffectBase_MouseAim::GetUseType() const
{
	return EItemUseType::MouseAim;
}

FVector UItemEffectBase_MouseAim::GetAimDirection() const
{
	return AimDirection;
}

void UItemEffectBase_MouseAim::UpdateContext(FItemUseContext& Context)
{
	Super::UpdateContext(Context);
	Context.Aimdirection = AimDirection;
}

bool UItemEffectBase_MouseAim::ChackTimeout()
{
	return ElapsedTime >= TimeLimt;
}
