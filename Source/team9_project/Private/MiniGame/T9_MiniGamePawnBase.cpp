#include "MiniGame/T9_MiniGamePawnBase.h"

AT9_MiniGamePawnBase::AT9_MiniGamePawnBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AT9_MiniGamePawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AT9_MiniGamePawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AT9_MiniGamePawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

