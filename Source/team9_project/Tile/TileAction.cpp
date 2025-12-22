// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileAction.h"
#include "Player/PlayerCharacter.h"

void UTileAction::Active(APlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("UTileAction::Active"));
}
