// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileAction/ServerTileAction/DecreaseHPTileAction.h"
#include "Player/PlayerCharacter.h"
#include "Player/MyPlayerState.h"
#include "Tile/Tile.h"
#include "Kismet/GameplayStatics.h"

void UDecreaseHPTileAction::Active(APlayerCharacter* PlayerCharacter) {
	Super::Active(PlayerCharacter);

	AMyPlayerState* PlayerState = Cast<AMyPlayerState>(PlayerCharacter->GetPlayerState());
	ATile* Tile = GetTypedOuter<ATile>();
	UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, nullptr, Tile, UDamageType::StaticClass());
}