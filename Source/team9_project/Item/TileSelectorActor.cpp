// Cho_SungMin - 타일 선택 시각화 액터

#include "Item/TileSelectorActor.h"
#include "Components/StaticMeshComponent.h"

ATileSelectorActor::ATileSelectorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cho_SungMin - 네트워크 복제 활성화
	bReplicates = true;
	bAlwaysRelevant = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectorMesh"));
	SelectorMesh->SetupAttachment(RootComp);

	// 기본 메시 설정 (실린더 또는 화살표)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshFinder.Succeeded())
	{
		SelectorMesh->SetStaticMesh(MeshFinder.Object);
		SelectorMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 0.1f));
	}

	// 기본 머티리얼 색상 설정
	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(
		TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		SelectorMesh->SetMaterial(0, MatFinder.Object);
	}

	// 충돌 비활성화
	SelectorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATileSelectorActor::BeginPlay()
{
	Super::BeginPlay();

	BaseLocation = GetActorLocation();
	TargetLocation = BaseLocation;
}

void ATileSelectorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAccumulator += DeltaTime;

	// 부유 애니메이션
	float FloatOffset = FMath::Sin(TimeAccumulator * FloatSpeed) * FloatAmplitude;
	FVector NewLocation = BaseLocation;
	NewLocation.Z += HeightOffset + FloatOffset;
	SetActorLocation(NewLocation);

	// 회전 애니메이션
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(CurrentRotation);
}

void ATileSelectorActor::MultiRPC_SetTargetLocation_Implementation(FVector NewLocation)
{
	TargetLocation = NewLocation;
	BaseLocation = NewLocation;

	FVector ActualLocation = NewLocation;
	ActualLocation.Z += HeightOffset;
	SetActorLocation(ActualLocation);
}

void ATileSelectorActor::MultiRPC_SetVisible_Implementation(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorTickEnabled(bVisible);
}
