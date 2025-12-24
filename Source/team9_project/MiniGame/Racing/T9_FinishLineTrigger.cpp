#include "MiniGame/Racing/T9_FinishLineTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"  
#include "MiniGame\Racing/T9_RacingGameMode.h"

AT9_FinishLineTrigger::AT9_FinishLineTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; 

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FinishTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FinishTrigger"));
	FinishTrigger->SetupAttachment(Root);
	FinishTrigger->SetBoxExtent(FVector(200.f, 500.f, 200.f));

	FinishTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FinishTrigger->SetCollisionObjectType(ECC_WorldStatic);
	FinishTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	FinishTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AT9_FinishLineTrigger::OnFinishOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
		return;

	AController* Controller = Pawn->GetController();
	if (!Controller)
		return;

	AMyPlayerState* PS = Controller->GetPlayerState<AMyPlayerState>();
	if (!PS)
		return;

	AT9_RacingGameMode* GM = GetWorld()->GetAuthGameMode<AT9_RacingGameMode>();
	if (!GM)
		return;

	GM->Winner=PS->GetPlayerNumber();
	GM->EndGame();
}

void AT9_FinishLineTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FinishTrigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&AT9_FinishLineTrigger::OnFinishOverlap
		);
	}
}

void AT9_FinishLineTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

