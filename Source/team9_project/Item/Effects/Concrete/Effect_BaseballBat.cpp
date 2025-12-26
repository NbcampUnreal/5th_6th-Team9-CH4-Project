// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_BaseballBat.h"
#include "Item/Data/ItemUseContext.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

void UEffect_BaseballBat::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
    Super::ExecuteEffect(User, Context);

    if (!User || !User->GetWorld()) return;

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
    QueryParams.AddIgnoredActor(User);

    bool bHasHit = User->GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        AttackCenter,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(AttackRadius),
        QueryParams
    );

#if WITH_EDITOR
    // 공격 방향 라인
    DrawDebugLine(
        User->GetWorld(),
        UserLocation,
        AttackCenter,
        FColor::Yellow,
        false,
        3.0f,
        0,
        2.0f
    );
    
    // 공격 범위 디버그 구체
    DrawDebugSphere(
        User->GetWorld(),
        AttackCenter,
        AttackRadius,
        16,
        bHasHit ? FColor::Red : FColor::Green,
        false,
        3.0f,
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

        // PlayerCharacter만 타격
        APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(HitActor);
        if (!TargetPlayer)
        {
            UE_LOG(LogTemp, Log, TEXT("Effect_BaseballBat: Skipping non-player %s"), *HitActor->GetName());
            continue;
        }

        UGameplayStatics::ApplyDamage(
            TargetPlayer,
            Damage,
            User->GetInstigatorController(),
            User,
            nullptr
        );

        HitCount++;
        UE_LOG(LogTemp, Log, TEXT("Effect_BaseballBat: Hit %s for %.0f damage"), *TargetPlayer->GetName(), Damage);
    }
}