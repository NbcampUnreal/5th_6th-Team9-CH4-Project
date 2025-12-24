// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_BrokenTeleporter.h"
#include "Item/Data/ItemUseContext.h"
#include "Tile/Tile.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UEffect_BrokenTeleporter::ExecuteEffect(AActor* User, const FItemUseContext& Context)
{
	Super::ExecuteEffect(User, Context);

	if (!User || !User->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_BrokenTeleporter: User or World is null"));
		return;
	}

	// 월드에서 모든 타일 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(User->GetWorld(), ATile::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_BrokenTeleporter: No tiles found in world"));
		return;
	}

	// 랜덤 타일 선택
	int32 RandomIndex = FMath::RandRange(0, FoundActors.Num() - 1);
	ATile* RandomTile = Cast<ATile>(FoundActors[RandomIndex]);

	if (!RandomTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect_BrokenTeleporter: Failed to cast to ATile"));
		return;
	}

	// 텔레포트 위치 계산
	FVector TeleportLocation = RandomTile->GetActorLocation();
	TeleportLocation.Z += 46.0f; // PlayerCharacter.cpp와 동일한 높이 오프셋

	// 위치 이동
	User->SetActorLocation(TeleportLocation);

	// PlayerCharacter인 경우 CurrentTile 업데이트
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(User))
	{
		PlayerChar->SetCurrentTile(RandomTile);
	}

	UE_LOG(LogTemp, Log, TEXT("Effect_BrokenTeleporter: %s teleported to random tile (index: %d)"),
		*User->GetName(), RandomTile->GetIndex());
}
