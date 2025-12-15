#include "Player/TestNode.h"


ATestNode::ATestNode()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

void ATestNode::BeginPlay()
{
	Super::BeginPlay();

	WorldLocation = GetActorLocation();
}

void ATestNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ATestNode* ATestNode::GetNextTile()
{
	return NextNode;
}
