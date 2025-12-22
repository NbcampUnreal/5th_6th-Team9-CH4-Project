// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class APlayerCharacter;
class AMyPlayerController;
class AMyPlayerState;
class UPlayerStateMachine;
class UItemEffectBase;
struct FInputActionValue;

UCLASS()
class TEAM9_PROJECT_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewControlle) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	bool GetIsUsingItem();

	UFUNCTION(Server, Reliable)
	void ServerRPCLeftClick();

	UFUNCTION(Server, Reliable)
	void ServerRPCRightClick();

	// 내화면에만 움직이면 됌
	void CameraKeyMoveHandle(const FInputActionValue&);
	void CameraWheelHandle(const FInputActionValue&);
	void CameraReturnHandle(const FInputActionValue&);

	void LeftClickHandle(const FInputActionValue&);
	void RightClickHandle(const FInputActionValue&);


	void ItemUseStart();
	void ItemUseEnd();

protected:
	UPROPERTY()
	USceneComponent* Root;
	UPROPERTY()
	USpringArmComponent* SpringArmComp;
	UPROPERTY()
	UCameraComponent* CameraComp;


	UPROPERTY()
	AMyPlayerController* MyPlayerController;

	// 케릭터 스폰용
	UPROPERTY()
	TSubclassOf<APlayerCharacter> CharacterClass;
	// 스폰 케릭터 관리
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;


	UPROPERTY()
	UPlayerStateMachine* StateMachine;

	
	// 화면 이동용
	UPROPERTY(EditAnywhere)
	float ScreenSpeed;
	UPROPERTY(EditAnywhere)
	float EdgeSize;
	int32 ViewX;
	int32 ViewY;
	float MouseX;
	float MouseY;

	// 아이템 사용중 체크
	bool bisUsingItem;
};
