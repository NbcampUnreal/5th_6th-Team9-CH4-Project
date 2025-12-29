// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_BrokenTeleporter.h"
#include "Item/Data/ItemUseContext.h"
#include "Tile/Tile.h"
#include "Tile/TileManagerActor.h"
#include "Player/PlayerCharacter.h"
#include "Player/CameraPawn.h"

void UEffect_BrokenTeleporter::ExecuteEffect(AActor* User, const FItemUseContext& Context)
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

	int32 TileCount = TileManager->GetTileCount();
	if (TileCount == 0)
	{
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, TileCount - 1);
	ATile* RandomTile = TileManager->GetTile(RandomIndex);

	if (!RandomTile)
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
		PlayerChar->MultiRPC_Teleport(RandomIndex);
	}
}
