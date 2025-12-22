#include "Player/CameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "GameMode/MainGameMode.h"
#include "Player/MyPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"
#include "State/PlayerStateMachine.h"
#include "State/StateBase.h"
#include "Tile/Tile.h"
#include "Kismet/GameplayStatics.h"

ACameraPawn::ACameraPawn() :
	ScreenSpeed(1500.f),
	EdgeSize(50.f),
	bisUsingItem(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(Root);
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->TargetArmLength = 800.f;
	SpringArmComp->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	static ConstructorHelpers::FClassFinder<APlayerCharacter> PC(TEXT("/Game/Blueprint/Player/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (PC.Succeeded())
	{
		CharacterClass = PC.Class;
	}
}

void ACameraPawn::PossessedBy(AController* NewControlle)
{
	Super::PossessedBy(NewControlle);

	if (HasAuthority())
	{
		SetOwner(NewControlle);
		UE_LOG(LogTemp, Warning, TEXT("Owner set to %s"), *GetNameSafe(NewControlle));
	}

	MyPlayerController = Cast<AMyPlayerController>(GetController());
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
		(CharacterClass, GetActorLocation(), GetActorRotation());
	PlayerCharacter->InitCharacter(this);

	StateMachine = NewObject<UPlayerStateMachine>(this);
	if (IsValid(StateMachine))
	{
		StateMachine->Initialize(this, PlayerCharacter);
	}

	APlayerState* PS = GetPlayerState();
	if (IsValid(PS))
	{
		AMyPlayerState* TPS = Cast<AMyPlayerState>(PS);
		if (IsValid(TPS))
		{
			PlayerCharacter->SetPlayerState(TPS);
		}
	}
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent Failed"))
		return;
	}

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EIC))
	{
		UE_LOG(LogTemp, Warning, TEXT("UEnhancedInputComponent"))
		if (IsValid(MyPlayerController))
		{
			UE_LOG(LogTemp, Warning, TEXT("MyPlayerController"))
			//BindAction
			EIC->BindAction(
				MyPlayerController->LeftClickAction,
				ETriggerEvent::Started,
				this,
				&ACameraPawn::LeftClickHandle);

			EIC->BindAction(
				MyPlayerController->RightClickAction,
				ETriggerEvent::Started,
				this,
				&ACameraPawn::RightClickHandle);

			EIC->BindAction(
				MyPlayerController->CameraKeyMoveAction,
				ETriggerEvent::Triggered,
				this,
				&ACameraPawn::CameraKeyMoveHandle);

			EIC->BindAction(
				MyPlayerController->CameraWheelAction,
				ETriggerEvent::Started,
				this,
				&ACameraPawn::CameraWheelHandle);

			EIC->BindAction(
				MyPlayerController->CameraReturnAction,
				ETriggerEvent::Started,
				this,
				&ACameraPawn::CameraReturnHandle);
		}
	}
}

void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(StateMachine))
	{
		StateMachine->OnUpdate(DeltaTime);
	}

	//내화면에서만 적용
	if (IsLocallyControlled() == false)
	{
		return;
	}

	if (IsValid(MyPlayerController))
	{
		if (ViewX == 0 || ViewY == 0)
		{
			MyPlayerController->GetViewportSize(ViewX, ViewY);
			UE_LOG(LogTemp, Warning, TEXT("ViewX : %d"), ViewX);
			UE_LOG(LogTemp, Warning, TEXT("ViewX : %d"), ViewY);
		}
		
		MyPlayerController->GetMousePosition(MouseX, MouseY);

		FVector2D MoveInput(0.f, 0.f);

		if (MouseX <= EdgeSize)
			MoveInput.Y = -1.f;
		else if (MouseX >= ViewX - EdgeSize)
			MoveInput.Y = 1.f;

		if (MouseY <= EdgeSize)
			MoveInput.X = 1.f;
		else if (MouseY >= ViewY - EdgeSize)
			MoveInput.X = -1.f;

		if (!MoveInput.IsNearlyZero())
		{
			const FRotator YawRotation(0.f, GetActorRotation().Yaw, 0.f);

			const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			FVector MoveDirection = Forward * MoveInput.X + Right * MoveInput.Y;

			AddActorWorldOffset(MoveDirection * ScreenSpeed * DeltaTime, true);
		}
	}
}

void ACameraPawn::LeftClickHandle(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: LeftClick"));

	ServerRPCLeftClick();
}

void ACameraPawn::RightClickHandle(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: RightClick"));

	ServerRPCRightClick();
}

void ACameraPawn::ItemUseStart()
{
	bisUsingItem = true;
	StateMachine->GetCurrentState()->ItemUse();
}

void ACameraPawn::ItemUseEnd()
{
	bisUsingItem = false;
} 

bool ACameraPawn::GetIsUsingItem()
{
	return bisUsingItem;
}

void ACameraPawn::CameraKeyMoveHandle(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: CameraKeyMoveHandle"));

	if (IsLocallyControlled() == false)
	{
		return;
	}

	const FVector2D ArrowInput = Value.Get<FVector2D>();

	if (ArrowInput.IsNearlyZero())
	{
		return;
	}
	
	const FRotator YawRotation(0.f, GetActorRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector MoveDirection = Forward * ArrowInput.X + Right * ArrowInput.Y;
	AddActorWorldOffset(MoveDirection * ScreenSpeed * DeltaTime, true);
}

void ACameraPawn::CameraWheelHandle(const FInputActionValue& Value)
{
	if (IsLocallyControlled() == false)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CLIENT: CameraWheelHandle"));
}

void ACameraPawn::CameraReturnHandle(const FInputActionValue&)
{
	if (IsLocallyControlled() == false)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CLIENT: CameraReturnHandle"));
	SetActorLocation(PlayerCharacter->GetActorLocation());
}

// 이동 테스트
void ACameraPawn::ServerRPCLeftClick_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Sever : LeftClick"));
	
	StateMachine->GetCurrentState()->Move();
}

// 공격받기 테스트
void ACameraPawn::ServerRPCRightClick_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Sever : RightClick"));

	StateMachine->GetCurrentState()->Hit();

	UGameplayStatics::ApplyDamage(
		PlayerCharacter,
		50.f,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
}
