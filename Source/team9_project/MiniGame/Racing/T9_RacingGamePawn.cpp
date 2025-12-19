#include "MiniGame/Racing/T9_RacingGamePawn.h"
#include "MiniGame/Racing/T9_RacingGamePlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AT9_RacingGamePawn::AT9_RacingGamePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("Pawn"));
	Mesh->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(Root);
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = true; 
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 10.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void AT9_RacingGamePawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
				LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				SubSystem->AddMappingContext(IMC_Racing, 0);
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("ButtonClick"));
}

void AT9_RacingGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		SimulateMove(DeltaTime);
	}
	if (HasAuthority())
	{
		SimulateMove(DeltaTime);
	}
}

void AT9_RacingGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EI->BindAction(IA_Forward, ETriggerEvent::Triggered, this, &AT9_RacingGamePawn::Input_Forward);

		EI->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AT9_RacingGamePawn::Input_Turn);

		EI->BindAction(IA_Turn, ETriggerEvent::Completed, this, &AT9_RacingGamePawn::Input_Turn);
	}
}

void AT9_RacingGamePawn::Input_Forward(const FInputActionValue& Value)
{
	if (!bInputEnabled) return;

	Server_Forward();
}

void AT9_RacingGamePawn::Input_Turn(const FInputActionValue& Value)
{
	if (!bInputEnabled)
	{
		TurnInput = 0.f;
		return;
	}

	TurnInput = Value.Get<float>();
	Server_Turn(TurnInput);
}

void AT9_RacingGamePawn::SimulateMove(float DeltaTime)
{
	CurrentSpeed = FMath::Max(0.f, CurrentSpeed - Decel * DeltaTime);

	AddActorWorldOffset(
		GetActorForwardVector() * CurrentSpeed * DeltaTime,
		true
	);

	if (!FMath::IsNearlyZero(TurnInput))
	{
		FRotator Rot = GetActorRotation();
		Rot.Yaw += TurnInput * TurnSpeed * DeltaTime;
		SetActorRotation(Rot);
	}
}

void AT9_RacingGamePawn::Server_Turn_Implementation(float Value)
{
	TurnInput = Value;
}

void AT9_RacingGamePawn::Server_Forward_Implementation()
{
	CurrentSpeed = FMath::Clamp(CurrentSpeed + AccelPerPress, 0.f, MaxSpeed);
}
