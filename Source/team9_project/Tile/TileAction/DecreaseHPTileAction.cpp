// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileAction/DecreaseHPTileAction.h"
#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"

void UDecreaseHPTileAction::Active(APlayerCharacter* PlayerCharacter) {
	Super::Active(PlayerCharacter);

	AMyPlayerState* PlayerState = Cast<AMyPlayerState>(PlayerCharacter->GetPlayerState());
	UE_LOG(LogTemp, Warning, TEXT("Before HP : %d"), PlayerState->GetHP());
	PlayerState->SetHP(PlayerState->GetHP() - Damage);
	UE_LOG(LogTemp, Warning, TEXT("After HP : %d"), PlayerState->GetHP());
}