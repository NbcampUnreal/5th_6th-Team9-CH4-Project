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
#include "Net/UnrealNetwork.h"
// Cho_Sungmin
#include "Inventory/InventoryComponent.h"
#include "Item/Effects/ItemEffectBase_DirectControl.h"

ACameraPawn::ACameraPawn() :
	ScreenSpeed(1500.f),
	EdgeSize(50.f),
	bisUsingItem(false)
{
	PrimaryActorTick.bCanEverTick = true;

	// Cho_Sungmin - InventoryComponent 생성
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

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

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ACameraPawn::BeginPlay"));

	if (HasAuthority())
	{
		PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>
			(CharacterClass, GetActorLocation(), GetActorRotation());


		AMyPlayerState* PS = Cast<AMyPlayerState>(GetPlayerState());

		PlayerCharacter->InitCharacter(this, PS);
	}
	
	
	
}

void ACameraPawn::PossessedBy(AController* NewControlle)
{
	Super::PossessedBy(NewControlle);
	UE_LOG(LogTemp, Warning, TEXT("=== PossessedBy === Pawn: %s, Controller: %s"), 
		*GetName(), *GetNameSafe(NewControlle));

	if (HasAuthority())
	{
		SetOwner(NewControlle);
        
		if (InventoryComponent)
		{
			InventoryComponent->AddItem(FName("RCCar"));
			InventoryComponent->AddItem(FName("BaseballBat"));
			InventoryComponent->AddItem(FName("Shotgun"));
			InventoryComponent->AddItem(FName("RCCar"));
			UE_LOG(LogTemp, Warning, TEXT("ADDITEM for %s"), *GetName());
		}
	}

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
		MyPlayerController = Cast<AMyPlayerController>(GetController());
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

void ACameraPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACameraPawn, PlayerCharacter);
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

	// 아이템 사용 중이면 타겟 따라가기
	if (InventoryComponent && InventoryComponent->IsUsingItem())
	{
		UpdateMouseAim();
		FVector TargetLocation = GetItemCameraTargetLocation();
		FVector CurrentLocation = GetActorLocation();
		
		FVector NewLocation = FMath::VInterpTo(
			CurrentLocation, 
			FVector(TargetLocation.X, TargetLocation.Y, CurrentLocation.Z),
			DeltaTime, 
			5.0f  // 추적 속도
		);
		SetActorLocation(NewLocation);
		return;  // 마우스 엣지 이동 스킵
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
	// 아이템 사용 중이면 확정
	if (InventoryComponent && InventoryComponent->IsUsingItem())
	{
		ServerRPC_ConfirmItemUse();
		return;
	}

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
	// Cho_Sungmin
	StateMachine->GetCurrentState()->ItemUse();
}

void ACameraPawn::ItemUseEnd()
{
	bisUsingItem = false;
}

bool ACameraPawn::GetIsUsingItem()
{
	// Cho_Sungmin - InventoryComponent 연동
	if (InventoryComponent)
	{
		return InventoryComponent->IsUsingItem();
	}
	return bisUsingItem;
}

// Cho_Sungmin - InventoryComponent Getter
UInventoryComponent* ACameraPawn::GetInventoryComponent() const
{
	return InventoryComponent;
}

// Cho_Sungmin - 아이템 사용
bool ACameraPawn::UseItem(int32 SlotIndex)
{
	if (!InventoryComponent)
	{
		return false;
	}

	bool bSuccess = InventoryComponent->UseItem(SlotIndex);
	if (bSuccess)
	{
		// 조작형 아이템인 경우 ItemUseState로 전환
		if (InventoryComponent->IsUsingItem())
		{
			StateMachine->GetCurrentState()->ItemUse();
		}
	}
	return bSuccess;
}

// Cho_Sungmin - 아이템 사용 확정
void ACameraPawn::ConfirmItemUse()
{
	if (InventoryComponent)
	{
		InventoryComponent->ConfirmItemUse();
	}
}

// Cho_Sungmin - 아이템 사용 취소
void ACameraPawn::CancelItemUse()
{
	if (InventoryComponent)
	{
		InventoryComponent->CancelItemUse();
	}
}

// Cho_Sungmin - PlayerCharacter 접근
APlayerCharacter* ACameraPawn::GetPlayerCharacter() const
{
	return PlayerCharacter;
}

void ACameraPawn::CameraKeyMoveHandle(const FInputActionValue& Value)
{
	

	if (IsLocallyControlled() == false)
	{
		return;
	}

	const FVector2D ArrowInput = Value.Get<FVector2D>();
	

	// Cho_Sungmin 아이템 사용 중이면 서버로 입력 전달
	if (InventoryComponent && InventoryComponent->IsUsingItem())
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> Sending to ServerRPC! Input: %s"), *ArrowInput.ToString());
		ServerRPC_SetItemControlInput(ArrowInput);
		return;
	}
	
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

// Cho_Sungmin Server RPC 구현
void ACameraPawn::ServerRPC_SetItemControlInput_Implementation(FVector2D Input)
{
	
    
	if (InventoryComponent)
	{
		InventoryComponent->SetDirectControlInput(Input);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is NULL!"));
	}
}
// Cho_Sungmin 카메라 추적 타겟 위치
FVector ACameraPawn::GetItemCameraTargetLocation() const
{
	if (!InventoryComponent || !InventoryComponent->IsUsingItem())
	{
		return GetActorLocation();
	}

	// Replicated된 ControlledActor 사용
	AActor* ControlledActor = InventoryComponent->CurrentControlledActor;
	if (ControlledActor)
	{
		return ControlledActor->GetActorLocation();
	}

	return GetActorLocation();
}

void ACameraPawn::UpdateMouseAim()
{
	if (!MyPlayerController) return;
    
	FHitResult HitResult;
	bool bHit = MyPlayerController->GetHitResultUnderCursor(
		ECC_Visibility,
		false,
		HitResult
	);
    
	
    
	if (bHit && PlayerCharacter)
	{
		FVector CharacterLocation = PlayerCharacter->GetActorLocation();
		FVector MouseWorldLocation = HitResult.Location;
        
		
        
		FVector Direction = MouseWorldLocation - CharacterLocation;
		Direction.Z = 0;
		Direction.Normalize();
		
        
		if (!Direction.IsNearlyZero())
		{
			ServerRPC_SetMouseAimDirection(Direction);
		}
	}
}

void ACameraPawn::ServerRPC_SetMouseAimDirection_Implementation(FVector Direction)
{
	if (InventoryComponent)
	{
		InventoryComponent->SetMouseAimDirection(Direction);
	}
}

void ACameraPawn::ServerRPC_ConfirmItemUse_Implementation()
{
	if (InventoryComponent)
	{
		InventoryComponent->ConfirmItemUse();
	}
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
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: CameraReturnHandle"));

	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: PlayerCharacter not"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("CLIENT: PlayerCharacter"));
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
