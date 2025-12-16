#include "MiniGame/T9_MiniGamePawnBase.h"

AT9_MiniGamePawnBase::AT9_MiniGamePawnBase()
{
	bInputEnabled = false;
}

void AT9_MiniGamePawnBase::BeginPlay()
{
	Super::BeginPlay();
	DisablePawnInput();
}

void AT9_MiniGamePawnBase::EnablePawnInput()
{
	bInputEnabled = true;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		EnableInput(PC);
	}
}

void AT9_MiniGamePawnBase::DisablePawnInput()
{
	bInputEnabled = false;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
}

void AT9_MiniGamePawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

