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
// Cho_Sungmin
class UInventoryComponent;

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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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

	// Cho_Sungmin - InventoryComponent 접근
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const;

	// Cho_Sungmin - 아이템 사용 (SlotIndex 지정)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 SlotIndex);

	// Cho_Sungmin - 아이템 사용 확정 (조작형 아이템용)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ConfirmItemUse();

	// Cho_Sungmin - 아이템 사용 취소
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CancelItemUse();

	// Cho_Sungmin - PlayerCharacter 접근
	UFUNCTION(BlueprintCallable, Category = "Character")
	APlayerCharacter* GetPlayerCharacter() const;
	
	// Cho_Sungmin - 아이템 조작 입력용 Server RPC
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemControlInput(FVector2D Input);

	// Cho_Sungmin - 카메라 추적 타겟 위치 가져오기
	FVector GetItemCameraTargetLocation() const;
	
	// Cho_Sungmin - 마우스 에임 업데이트
	void UpdateMouseAim();
	
	// Server RPC로 조준 방향 전달
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetMouseAimDirection(FVector Direction);

	// Server RPC로 아이템 확정
	UFUNCTION(Server, Reliable)
	void ServerRPC_ConfirmItemUse();
	

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
	UPROPERTY(Replicated)
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

	// Cho_Sungmin
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UInventoryComponent* InventoryComponent;
};
