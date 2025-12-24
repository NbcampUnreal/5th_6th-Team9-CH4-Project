// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_BaseballBat.h"
#include "Item/Data/ItemUseContext.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void UEffect_BaseballBat::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_BaseballBat: User or World is null"));
		return;
	}

	// AimDirection 사용
	FVector Direction = AimDirection.GetSafeNormal();

	if (Direction.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_BaseballBat: AimDirection is zero"));
		return;
	}

	// 공격 중심 위치 계산 (User 위치 + 방향 * 거리)
	FVector UserLocation = User->GetActorLocation();
	FVector AttackCenter = UserLocation + Direction * AttackRange;

	// SphereOverlap으로 범위 내 액터 찾기
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(User); // 자신은 무시

	bool bHasHit = User->GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		AttackCenter,
		FQuat::Identity,
		ECC_Pawn, // Pawn 채널 사용
		FCollisionShape::MakeSphere(AttackRadius),
		QueryParams
	);

#if WITH_EDITOR
	// 디버그 구체 그리기
	DrawDebugSphere(
		User->GetWorld(),
		AttackCenter,
		AttackRadius,
		16,
		bHasHit ? FColor::Red : FColor::Yellow,
		false,
		2.0f,
		0,
		2.0f
	);
#endif

	// 적중한 액터들에게 데미지 적용
	int32 HitCount = 0;
	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor || HitActor == User)
		{
			continue;
		}

		// 데미지 적용
		UGameplayStatics::ApplyDamage(
			HitActor,
			Damage,
			User->GetInstigatorController(),
			User,
			nullptr
		);

		HitCount++;
		UE_LOG(LogTemp, Log, TEXT("Effect_BaseballBat: Hit %s for %.0f damage"), *HitActor->GetName(), Damage);
	}

	UE_LOG(LogTemp, Log, TEXT("Effect_BaseballBat: %s hit %d targets"), *User->GetName(), HitCount);
}
