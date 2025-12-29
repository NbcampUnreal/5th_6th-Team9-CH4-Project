// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileAction/ServerTileAction/GainScoreTileAction.h"
#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"

void UGainScoreTileAction::Active(APlayerCharacter* PlayerCharacter)
{
	Super::Active(PlayerCharacter);

	AMyPlayerState* PlayerState = Cast<AMyPlayerState>(PlayerCharacter->GetPlayerState());
	UE_LOG(LogTemp, Warning, TEXT("Before Score : %d"), PlayerState->GetScore());
	PlayerState->SetScore(PlayerState->GetScore() + Score);
	UE_LOG(LogTemp, Warning, TEXT("After Score : %d"), PlayerState->GetScore());
}
