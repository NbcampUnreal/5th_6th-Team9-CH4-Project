#include "Player/PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerController.h"
#include "State/PlayerStateMachine.h"
#include "State/StateBase.h"
#include "Tile/Tile.h"
#include "EnhancedInputComponent.h"
#include "CameraPawn.h"


APlayerCharacter::APlayerCharacter() :
	MoveSpeed(300.f),
	MoveDuration(0.f),
	MoveElapsed(0.f),
	remainingMove(0)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (IsValid(StateMachine))
	{
		StateMachine->OnUpdate(DeltaTime);
	}*/
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
	{
		return 0.f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), ActualDamage);
	/*if (!StateMachine || !StateMachine->GetCurrentState()->CanTakeDamage())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dont TakeDamage"));
		return 0.f;
	}

	MyPlayerState->CurrentHp = FMath::Clamp(MyPlayerState->CurrentHp - (int)ActualDamage, 0, MyPlayerState->MaxHp);
	UE_LOG(LogTemp, Warning, TEXT("Current HP : %d"), MyPlayerState->CurrentHp);
	if (MyPlayerState->CurrentHp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		OnDie();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		StateMachine->ChangeState(EStates::Hit);
	}*/

	return ActualDamage;
}

void APlayerCharacter::MoveToNextNode(int DiceValue)
{
	if (!HasAuthority())
	{
		return;
	}

	remainingMove = DiceValue;

	/*MoveStart = CurrentNode->WorldLocation;
	MoveTarget = CurrentNode->NextNode->WorldLocation;*/

	FVector ToTarget = (MoveTarget - MoveStart).GetSafeNormal();
	FRotator TargetRotation = ToTarget.Rotation();
	//MulticastRPCSetRotation(TargetRotation);
	//SetActorRotation(TargetRotation);


	float Distance = FVector::Dist(MoveStart, MoveTarget);
	MoveDuration = Distance / MoveSpeed;
	MoveElapsed = 0.f;

	// 0.01초 간격으로 이동 업데이트
	GetWorldTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&APlayerCharacter::UpdateMove,
		0.01f,
		true
	);
}

void APlayerCharacter::UpdateMove()
{
	MoveElapsed += 0.01f;

	float Alpha = FMath::Clamp(MoveElapsed / MoveDuration, 0.f, 1.f);
	FVector NewLocation = FMath::Lerp(MoveStart, MoveTarget, Alpha);
	SetActorLocation(FVector(NewLocation.X, NewLocation.Y, NewLocation.Z + 46));

	if (Alpha >= 1.f)
	{
		/*CurrentNode = CurrentNode->NextNode;
		SetActorLocation(CurrentNode->WorldLocation);*/
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);

		remainingMove--;
		if (remainingMove > 0)
		{
			MoveToNextNode(remainingMove);
		}
		else
		{
			//StateMachine->ChangeState(EStates::Idle);
		}
	}
}

void APlayerCharacter::OnDie()
{
	//StateMachine->ChangeState(EStates::Die);
}
