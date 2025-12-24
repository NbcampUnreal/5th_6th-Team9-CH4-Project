// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Beehive.h"
#include "Item/Data/ItemUseContext.h"
#include "Tile/Tile.h"
#include "Tile/TileManagerActor.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UEffect_Beehive::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Beehive: User or World is null"));
		return;
	}

	// 타일 매니저에서 타일 가져오기
	ATileManagerActor* TileManager = ATileManagerActor::Get(User->GetWorld());
	if (!TileManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Beehive: TileManager is null"));
		return;
	}

	int32 TargetIndex = Context.TargetTileIndex;
	if (TargetIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Beehive: Invalid TargetTileIndex: %d"), TargetIndex);
		return;
	}

	ATile* TargetTile = TileManager->GetTile(TargetIndex);
	if (!TargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_Beehive: TargetTile is null for index: %d"), TargetIndex);
		return;
	}

	// 타일에 있는 모든 플레이어 가져오기
	TArray<APlayerCharacter*> PlayersOnTile = TargetTile->GetInPlayers();

	int32 HitCount = 0;
	for (APlayerCharacter* Player : PlayersOnTile)
	{
		// 자기 자신은 제외
		if (!Player || Player == User)
		{
			continue;
		}

		// 데미지 적용
		UGameplayStatics::ApplyDamage(
			Player,
			Damage,
			User->GetInstigatorController(),
			User,
			nullptr
		);

		HitCount++;
		UE_LOG(LogTemp, Log, TEXT("Effect_Beehive: %s dealt %.0f damage to %s"),
			*User->GetName(), Damage, *Player->GetName());
	}

	UE_LOG(LogTemp, Log, TEXT("Effect_Beehive: Effect triggered on tile %d, hit %d players"),
		TargetIndex, HitCount);
}
