// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Shotgun.h"
#include "Item/Data/ItemUseContext.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void UEffect_Shotgun::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Shotgun: User or World is null"));
		return;
	}

	// AimDirection 사용 (Context가 아닌 클래스 멤버 변수)
	FVector BaseDirection = AimDirection.GetSafeNormal();

	if (BaseDirection.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Shotgun: AimDirection is zero"));
		return;
	}

	// 부채꼴로 여러 총알 발사
	for (int32 i = 0; i < BulletCount; ++i)
	{
		// 각 총알의 각도 계산 (-SpreadAngle ~ +SpreadAngle)
		float Angle = FMath::Lerp(-SpreadAngle, SpreadAngle, (float)i / FMath::Max(1, BulletCount - 1));

		// 각도만큼 방향 회전
		FVector RotatedDirection = BaseDirection.RotateAngleAxis(Angle, FVector::UpVector);

		// 총알 발사
		FireBullet(User, RotatedDirection);
	}

	UE_LOG(LogTemp, Log, TEXT("Effect_Shotgun: %s fired %d bullets"), *User->GetName(), BulletCount);
}

void UEffect_Shotgun::FireBullet(AActor* User, const FVector& Direction)
{
	if (!User || !User->GetWorld())
	{
		return;
	}

	// 발사 시작 위치
	FVector Start = User->GetActorLocation();
	Start.Z += 50.0f; // 캐릭터 중앙 높이

	// 발사 끝 위치
	FVector End = Start + Direction * Range;

	// LineTrace 설정
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(User); // 자신은 무시

	// LineTrace 실행
	bool bHit = User->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Pawn,
		QueryParams
	);

#if WITH_EDITOR
	// 디버그 라인 그리기
	DrawDebugLine(
		User->GetWorld(),
		Start,
		bHit ? HitResult.Location : End,
		bHit ? FColor::Red : FColor::Green,
		false,
		2.0f,
		0,
		2.0f
	);
#endif

	// 적중 시 데미지 적용
	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();

		UGameplayStatics::ApplyDamage(
			HitActor,
			DamagePerBullet,
			User->GetInstigatorController(),
			User,
			nullptr
		);

		UE_LOG(LogTemp, Log, TEXT("Effect_Shotgun: Hit %s for %.0f damage"), *HitActor->GetName(), DamagePerBullet);
	}
}
