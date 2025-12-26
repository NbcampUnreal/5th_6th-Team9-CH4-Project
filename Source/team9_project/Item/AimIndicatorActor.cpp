// Fill out your copyright notice in the Description page of Project Settings.


#include "AimIndicatorActor.h"
#include "Components/DecalComponent.h"
#include "Net/UnrealNetwork.h"

AAimIndicatorActor::AAimIndicatorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetupAttachment(RootComp);
	DecalComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f)); // 바닥을 향하게
}

void AAimIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAimIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePosition();
	UpdateRotation();
}

void AAimIndicatorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAimIndicatorActor, AimDirection);
}

void AAimIndicatorActor::SetAimDirection(FVector Direction)
{
	AimDirection = Direction.GetSafeNormal();
	UpdateRotation();
}

void AAimIndicatorActor::SetIndicatorSize(float Angle, float Range)
{
	if (DecalComp)
	{
		// Decal 크기 조절 (X=깊이, Y=너비, Z=높이)
		// Range를 기반으로 크기 설정
		DecalComp->DecalSize = FVector(Range, Range, Range);
	}
}

void AAimIndicatorActor::SetFollowTarget(AActor* Target)
{
	FollowTarget = Target;
}

void AAimIndicatorActor::OnRep_AimDirection()
{
	UpdateRotation();
}

void AAimIndicatorActor::UpdateRotation()
{
	if (!AimDirection.IsNearlyZero())
	{
		FRotator NewRotation = AimDirection.Rotation();
		
		SetActorRotation(FRotator(0.0f, NewRotation.Yaw + 90.0f, 0.0f));
	}
}

void AAimIndicatorActor::UpdatePosition()
{
	if (FollowTarget)
	{
		FVector TargetLocation = FollowTarget->GetActorLocation();
		TargetLocation.Z = HeightOffset; // 바닥 높이
		SetActorLocation(TargetLocation);
	}
}