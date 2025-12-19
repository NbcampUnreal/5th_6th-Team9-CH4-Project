#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TEAM9_PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	//void IMCChange(Mode mode);

public:
	UPROPERTY()
	UInputMappingContext* CurrentIMC;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* BoardIMC;

	UPROPERTY(EditAnywhere)
	UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere)
	UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere)
	UInputAction* CameraReturnAction;

	UPROPERTY(EditAnywhere)
	UInputAction* CameraKeyMoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* CameraWheelAction;
};
