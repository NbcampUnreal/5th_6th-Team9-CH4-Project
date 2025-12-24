// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Teleporter.h"
#include "Item/Data/ItemUseContext.h"
#include "Tile/Tile.h"
#include "Tile/TileManagerActor.h"
#include "Player/PlayerCharacter.h"

void UEffect_Teleporter::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Teleporter: User is null"));
		return;
	}

	// 타일 매니저에서 타일 가져오기
	ATileManagerActor* TileManager = ATileManagerActor::Get(User->GetWorld());
	if (!TileManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Teleporter: TileManager is null"));
		return;
	}

	int32 TargetIndex = Context.TargetTileIndex;
	if (TargetIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Teleporter: Invalid TargetTileIndex: %d"), TargetIndex);
		return;
	}

	ATile* TargetTile = TileManager->GetTile(TargetIndex);
	if (!TargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Teleporter: TargetTile is null for index: %d"), TargetIndex);
		return;
	}

	// 텔레포트 위치 계산
	FVector TeleportLocation = TargetTile->GetActorLocation();
	TeleportLocation.Z += 46.0f; // PlayerCharacter.cpp와 동일한 높이 오프셋

	// 위치 이동
	User->SetActorLocation(TeleportLocation);

	// PlayerCharacter인 경우 CurrentTile 업데이트
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(User))
	{
		PlayerChar->SetCurrentTile(TargetTile);
	}

	UE_LOG(LogTemp, Log, TEXT("Effect_Teleporter: %s teleported to tile %d"), *User->GetName(), TargetIndex);
}
