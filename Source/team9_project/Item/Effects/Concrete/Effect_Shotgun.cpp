// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Shotgun.h"
#include "Item/Data/ItemUseContext.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

void UEffect_Shotgun::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld()) return;

    FVector BaseDirection = AimDirection.GetSafeNormal();
    if (BaseDirection.IsNearlyZero()) return;

    FVector StartLocation = User->GetActorLocation();
    StartLocation.Z += 50.0f;

#if WITH_EDITOR
    // 부채꼴 범위 디버그
    for (int32 i = 0; i <= BulletCount; ++i)
    {
        float Angle = FMath::Lerp(-SpreadAngle, SpreadAngle, (float)i / FMath::Max(1, BulletCount));
        FVector RotatedDirection = BaseDirection.RotateAngleAxis(Angle, FVector::UpVector);
        FVector EndPoint = StartLocation + RotatedDirection * Range;
        
        DrawDebugLine(
            User->GetWorld(),
            StartLocation,
            EndPoint,
            FColor::Orange,
            false,
            3.0f,
            0,
            2.0f
        );
    }
    
    // 부채꼴 호(Arc) 그리기
    int32 ArcSegments = 16;
    for (int32 i = 0; i < ArcSegments; ++i)
    {
        float Angle1 = FMath::Lerp(-SpreadAngle, SpreadAngle, (float)i / ArcSegments);
        float Angle2 = FMath::Lerp(-SpreadAngle, SpreadAngle, (float)(i + 1) / ArcSegments);
        
        FVector Dir1 = BaseDirection.RotateAngleAxis(Angle1, FVector::UpVector);
        FVector Dir2 = BaseDirection.RotateAngleAxis(Angle2, FVector::UpVector);
        
        FVector Point1 = StartLocation + Dir1 * Range;
        FVector Point2 = StartLocation + Dir2 * Range;
        
        DrawDebugLine(
            User->GetWorld(),
            Point1,
            Point2,
            FColor::Orange,
            false,
            3.0f,
            0,
            2.0f
        );
    }
#endif

    // 기존 총알 발사 로직
    for (int32 i = 0; i < BulletCount; ++i)
    {
        float Angle = FMath::Lerp(-SpreadAngle, SpreadAngle, (float)i / FMath::Max(1, BulletCount - 1));
        FVector RotatedDirection = BaseDirection.RotateAngleAxis(Angle, FVector::UpVector);
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

		// PlayerCharacter만 타격
		APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(HitActor);
		if (!TargetPlayer)
		{
			return;
		}

		UGameplayStatics::ApplyDamage(
			TargetPlayer,
			DamagePerBullet,
			User->GetInstigatorController(),
			User,
			nullptr
		);

		UE_LOG(LogTemp, Log, TEXT("Effect_Shotgun: Hit %s for %.0f damage"), *TargetPlayer->GetName(), DamagePerBullet);
	}
}