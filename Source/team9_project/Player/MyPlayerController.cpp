#include "Player/MyPlayerController.h"

#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	//bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Possessed: %s"), *GetNameSafe(InPawn));
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 내 클라에서만 실행?
	if (!IsLocalController())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("APlayerController::BeginPlay"));

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem
		= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(BoardIMC, 0);
	CurrentIMC = BoardIMC;
}
