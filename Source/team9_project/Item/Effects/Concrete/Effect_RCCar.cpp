// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_RCCar.h"
#include "Item/Data/ItemUseContext.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Player/CameraPawn.h"
#include "Player/PlayerCharacter.h"

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

	// CameraPawn에서 PlayerCharacter 위치 가져오기
	FVector SpawnLocation;
	FRotator SpawnRotation;

	ACameraPawn* CameraPawn = Cast<ACameraPawn>(User);
	if (CameraPawn && CameraPawn->GetPlayerCharacter())
	{
		APlayerCharacter* PlayerChar = CameraPawn->GetPlayerCharacter();
		SpawnLocation = PlayerChar->GetActorLocation() + PlayerChar->GetActorForwardVector() * 100.0f;
		SpawnRotation = PlayerChar->GetActorRotation();
	}
	else
	{
		// fallback: User 위치 사용
		SpawnLocation = User->GetActorLocation() + User->GetActorForwardVector() * 100.0f;
		SpawnRotation = User->GetActorRotation();
	}

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

	FVector ExplosionLocation = ControlledActor->GetActorLocation();

	// SphereOverlap으로 폭발 범위 내 액터 찾기
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(User);
	QueryParams.AddIgnoredActor(ControlledActor);

	bool bHasHit = User->GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		ExplosionLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(ExplosionRadius),
		QueryParams
	);

	// 디버그 구체 그리기
#if WITH_EDITOR
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
#endif

	// 폭발 범위 내 PlayerCharacter에게만 데미지 적용
	int32 HitCount = 0;
	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor || HitActor == User)
		{
			continue;
		}

		// PlayerCharacter만 타겟
		APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(HitActor);
		if (!TargetPlayer)
		{
			UE_LOG(LogTemp, Log, TEXT("Effect_RCCar: Skipping non-player actor %s"), *HitActor->GetName());
			continue;
		}

		// 데미지 적용
		UGameplayStatics::ApplyDamage(
				TargetPlayer,
				ExplosionDamage,
				User->GetInstigatorController(),
				User,
				nullptr
		);

		HitCount++;
		UE_LOG(LogTemp, Log, TEXT("Effect_RCCar: Explosion hit player %s for %.0f damage"),
				*TargetPlayer->GetName(), ExplosionDamage);
	}

	// RC카 제거
	DestroyControlledActor();

	UE_LOG(LogTemp, Log, TEXT("Effect_RCCar: Explosion at %s, hit %d targets"),
		*ExplosionLocation.ToString(), HitCount);
}

void UEffect_RCCar::CancelUse()
{
	// 타임아웃 시 폭발 처리
	if (ControlledActor && CurrentUser)
	{
		UE_LOG(LogTemp, Log, TEXT("Effect_RCCar: Timeout - Exploding!"));

		
		FItemUseContext DummyContext;
		ExecuteEffect(CurrentUser, DummyContext);
	}

	// 부모 클래스 CancelUse 호출
	Super::CancelUse();
}
