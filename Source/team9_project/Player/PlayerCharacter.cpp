#include "Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "Player/TestNode.h"
#include "Player/MyPlayerController.h"
#include "State/PlayerStateMachine.h"
#include "State/StateBase.h"
#include "EnhancedInputComponent.h"


APlayerCharacter::APlayerCharacter() :
	MoveSpeed(300.f),
	MoveDuration(0.f),
	MoveElapsed(0.f),
	remainingMove(0)
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->TargetArmLength = 800.f;
	SpringArmComp->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Start Node Setting
	TArray<AActor*> FoundNodes;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Start"), FoundNodes);

	if (FoundNodes.Num() > 0)
	{
		SetCurrentNode(Cast<ATestNode>(FoundNodes[0]));
	}
	SetActorLocation(CurrentNode->WorldLocation);


	StateMachine = NewObject<UPlayerStateMachine>(this);
	if (IsValid(StateMachine))
	{
		StateMachine->Initialize(this);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(StateMachine))
	{
		StateMachine->OnUpdate(DeltaTime);
	}
}

void APlayerCharacter::PossessedBy(AController* NewControlle)
{
	Super::PossessedBy(NewControlle);

	if (HasAuthority())
	{
		SetOwner(NewControlle);
		UE_LOG(LogTemp, Warning, TEXT("Owner set to %s"), *GetNameSafe(NewControlle));
	}

	APlayerState* PS = GetPlayerState();
	if (IsValid(PS))
	{
		AMyPlayerState* TPS = Cast<AMyPlayerState>(PS);
		if (IsValid(TPS))
		{
			MyPlayerState = TPS;
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!IsLocallyControlled())
	{
		return;
	}

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EIC))
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
		if (IsValid(PlayerController))
		{
			//BindAction
			EIC->BindAction(
				PlayerController->LeftClickAction,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::LeftClickHandle);

			EIC->BindAction(
				PlayerController->RightClickAction,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::RightClickHandle);
		}
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
	{
		return 0.f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!StateMachine || !StateMachine->GetCurrentState()->CanTakeDamage())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dont TakeDamage"));
		return 0.f;
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), ActualDamage);

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
	}

	return ActualDamage;
}

void APlayerCharacter::SetCurrentNode(ATestNode* TileNode)
{
	CurrentNode = TileNode;
}

void APlayerCharacter::MoveToNextNode(int DiceValue)
{
	if (!HasAuthority())
	{
		return;
	}

	remainingMove = DiceValue;

	MoveStart = CurrentNode->WorldLocation;
	MoveTarget = CurrentNode->NextNode->WorldLocation;

	FVector ToTarget = (MoveTarget - MoveStart).GetSafeNormal();
	FRotator TargetRotation = ToTarget.Rotation();
	MulticastRPCSetRotation(TargetRotation);
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
		CurrentNode = CurrentNode->NextNode;
		SetActorLocation(CurrentNode->WorldLocation);
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);

		remainingMove--;
		if (remainingMove > 0)
		{
			MoveToNextNode(remainingMove);
		}
		else
		{
			StateMachine->ChangeState(EStates::Idle);
		}
	}
}

int APlayerCharacter::RandDice()
{
	int DiceValue = FMath::RandRange(1, 6);

	UE_LOG(LogTemp, Warning, TEXT("ATestCharacter::RandDice() Count : %d"), DiceValue);
	return DiceValue;
}

void APlayerCharacter::OnDie()
{
	StateMachine->ChangeState(EStates::Die);
}

void APlayerCharacter::LeftClickHandle(const FInputActionValue&)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: LeftClick"));

	ServerRPCLeftClick();
}

void APlayerCharacter::RightClickHandle(const FInputActionValue&)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: RightClick"));

	ServerRPCRightClick();
}

void APlayerCharacter::MulticastRPCSetRotation_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}

void APlayerCharacter::ServerRPCLeftClick_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Sever : LeftClick"));

	StateMachine->GetCurrentState()->Move();
}

void APlayerCharacter::ServerRPCRightClick_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Sever : RightClick"));

	UGameplayStatics::ApplyDamage(
		this,
		50.f,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
}

