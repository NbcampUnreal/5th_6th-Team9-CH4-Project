// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileAction/TileAction.h"
#include "Player/PlayerCharacter.h"

void UTileAction::Active(APlayerCharacter* PlayerCharacter)
{
	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		UE_LOG(LogTemp, Warning, TEXT("NM_Standalone, UTileAction::Active"));
	}

	if (GetWorld()->GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("NM_Client, UTileAction::Active"));
	}

	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("NM_DedicatedServer, UTileAction::Active"));
	}
}
