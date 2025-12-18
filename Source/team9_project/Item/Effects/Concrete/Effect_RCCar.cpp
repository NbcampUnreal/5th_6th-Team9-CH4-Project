// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_RCCar.h"
#include "Item/Data/ItemUseContext.h"
#include "Engine/World.h"  // SpawnActor에 필요
// #include "DrawDebugHelpers.h"  // TODO: 데미지 시스템 연동 시 활성화
// #include "Kismet/GameplayStatics.h"

AActor* UEffect_RCCar::SpawnControlledActor(AActor* User)
{
	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_RCCar: User or World is null"));
		return nullptr;
	}

	// RC카 클래스가 설정되지 않은 경우
	if (!RCCarClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_RCCar: RCCarClass is not set"));
		return nullptr;
	}

	// User 앞쪽에 RC카 스폰
	FVector SpawnLocation = User->GetActorLocation() + User->GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = User->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// RC카 스폰
	AActor* SpawnedCar = User->GetWorld()->SpawnActor<AActor>(RCCarClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedCar)
	{
		UE_LOG(LogTemp, Log, TEXT("Effect_RCCar: RC Car spawned at %s"), *SpawnLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_RCCar: Failed to spawn RC Car"));
	}

	return SpawnedCar;
}

void UEffect_RCCar::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_RCCar: User or World is null"));
		return;
	}

	// RC카가 없으면 종료
	if (!ControlledActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_RCCar: ControlledActor is null"));
		return;
	}

	if (!ControlledActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_RCCar: No ControlledActor to explode"));
		return;
	}

	FVector ExplosionLocation = ControlledActor->GetActorLocation();

	// TODO: 데미지 시스템 연동
	/*
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(User);
	QueryParams.AddIgnoredActor(ControlledActor);
	bool bHasHit = User->GetWorld()->OverlapMultiByChannel(...);
	for (const FOverlapResult& Result : OverlapResults)
	{
	    AActor* HitActor = Result.GetActor();
	    FDamageEvent DamageEvent;
	    HitActor->TakeDamage(ExplosionDamage, DamageEvent, User->GetInstigatorController(), User);
	}

	// 디버그 구체 그리기
	DrawDebugSphere(
		User->GetWorld(),
		ExplosionLocation,
		ExplosionRadius,
		16,
		FColor::Orange,
		false,
		3.0f,
		0,
		3.0f
	);
	*/

	// RC카 제거
	DestroyControlledActor();

	UE_LOG(LogTemp, Log, TEXT("Effect_RCCar: Explosion at %s (damage not implemented yet)"),
		*ExplosionLocation.ToString());
}
