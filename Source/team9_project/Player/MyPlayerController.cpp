#include "Player/MyPlayerController.h"

#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;

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

	UE_LOG(LogTemp, Warning, TEXT("ATestPlayerController::BeginPlay"));

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem
		= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(BoardIMC, 0);
	CurrentIMC = BoardIMC;

	if (IsValid(MainTitleWidgetClass))
	{
		MainTitleWidget = CreateWidget<UMainTitleWidget>(this, MainTitleWidgetClass);
		if (MainTitleWidget)
		{
			MainTitleWidget->AddToViewport();

			// UI 조작을 위한 마우스 커서 표시 및 입력 모드 설정
			// 이 설정은 로컬 플레이어에게만 적용
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());

			UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::BeginPlay - MainTitleWidget added to viewport."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMyPlayerController::BeginPlay - Failed to create MainTitleWidget."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMyPlayerController::BeginPlay - MainTitleWidgetClass is not valid. Please assign it in Blueprint."));
	}
}
