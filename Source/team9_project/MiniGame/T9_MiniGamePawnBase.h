// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "T9_MiniGamePawnBase.generated.h"

UCLASS()
class TEAM9_PROJECT_API AT9_MiniGamePawnBase : public APawn
{
	GENERATED_BODY()

public:
	AT9_MiniGamePawnBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void EnablePawnInput();

	virtual void DisablePawnInput();

protected:
	bool bInputEnabled;

};
