#include "MiniGame/T9_MiniGameCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AT9_MiniGameCharacterBase::AT9_MiniGameCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bInputEnabled = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AT9_MiniGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_MiniGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AT9_MiniGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AT9_MiniGameCharacterBase::EnableCharacterInput()
{
	bInputEnabled = true;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		EnableInput(PC);
	}
}

void AT9_MiniGameCharacterBase::DisableCharacterInput()
{
	bInputEnabled = false;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
}