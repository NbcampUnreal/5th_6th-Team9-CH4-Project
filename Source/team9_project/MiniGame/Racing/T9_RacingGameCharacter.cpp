#include "MiniGame/Racing/T9_RacingGameCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AT9_RacingGameCharacter::AT9_RacingGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void AT9_RacingGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::HandleMoveInput);

	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLookInput);
}

void AT9_RacingGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled() == true)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		checkf(IsValid(PC) == true, TEXT("PlayerController is invalid."));

		UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		checkf(IsValid(EILPS) == true, TEXT("EnhancedInputLocalPlayerSubsystem is invalid."));

		EILPS->AddMappingContext(InputMappingContext, 0);
	}
}

void AT9_RacingGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bAcceptInput)
	{
		return; 
	}
	if (bAccelRequested)
	{
		CurrentSpeed = FMath::Clamp(
			CurrentSpeed + AccelPerInput,
			0.f,
			MaxSpeed
		);
	}
	else
	{
		CurrentSpeed = FMath::Max(
			CurrentSpeed - DecelPerSecond * DeltaTime,
			0.f
		);
	}

	bAccelRequested = false;

	if (CurrentSpeed > 0.f && IsValid(Controller))
	{
		const FRotator ControlYaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Forward = FRotationMatrix(ControlYaw).GetUnitAxis(EAxis::X);

		AddMovementInput(Forward, CurrentSpeed * DeltaTime);
	}
}

void AT9_RacingGameCharacter::SetAcceptInput(bool bEnable)
{
	bAcceptInput = bEnable;
	UE_LOG(LogTemp, Error, TEXT("override"));
	if (!bAcceptInput)
	{
		CurrentSpeed = 0.f;
		bAccelRequested = false;
	}
}

void AT9_RacingGameCharacter::HandleMoveInput(const FInputActionValue& InValue)
{
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
		return;
	}

	const FVector2D InMovementVector = InValue.Get<FVector2D>();

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator ControlYawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	bAccelRequested = true;
	const FVector Location = GetActorLocation();
	UE_LOG(LogTemp, Warning,
		TEXT("Location = X: %.2f | Y: %.2f | Z: %.2f"),
		Location.X, Location.Y, Location.Z
	);
}

void AT9_RacingGameCharacter::HandleLookInput(const FInputActionValue& InValue)
{
	if (!bAcceptInput)
	{
		return;
	}
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
		return;
	}

	const FVector2D InLookVector = InValue.Get<FVector2D>();

	AddControllerYawInput(InLookVector.X);
}




