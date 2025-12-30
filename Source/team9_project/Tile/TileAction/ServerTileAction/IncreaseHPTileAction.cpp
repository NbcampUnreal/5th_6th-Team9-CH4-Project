// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileAction/ServerTileAction/IncreaseHPTileAction.h"
#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"

void UIncreaseHPTileAction::Active(APlayerCharacter* PlayerCharacter)
{
	Super::Active(PlayerCharacter);
	AMyPlayerState* PlayerState = Cast<AMyPlayerState>(PlayerCharacter->GetPlayerState());
	int32 PlayerHP = PlayerState->GetHP();
	PlayerState->SetHP(PlayerHP + Amount);
}
