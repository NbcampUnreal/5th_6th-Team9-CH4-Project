// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Teleporter.h"
#include "Item/Data/ItemUseContext.h"
#include "Tile/Tile.h"
#include "Tile/TileManagerActor.h"
#include "Player/PlayerCharacter.h"
#include "Player/CameraPawn.h"

void UEffect_Teleporter::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		return;
	}

	ATileManagerActor* TileManager = ATileManagerActor::Get(User->GetWorld());
	if (!TileManager)
	{
		return;
	}

	int32 TargetIndex = Context.TargetTileIndex;
	if (TargetIndex < 0 || TargetIndex >= TileManager->GetTileCount())
	{
		return;
	}

	ATile* TargetTile = TileManager->GetTile(TargetIndex);
	if (!TargetTile)
	{
		return;
	}

	APlayerCharacter* PlayerChar = nullptr;
	ACameraPawn* CameraPawn = Cast<ACameraPawn>(User);
	if (CameraPawn)
	{
		PlayerChar = CameraPawn->GetPlayerCharacter();
	}

	if (PlayerChar)
	{
		PlayerChar->MultiRPC_Teleport(TargetIndex);
	}
}
