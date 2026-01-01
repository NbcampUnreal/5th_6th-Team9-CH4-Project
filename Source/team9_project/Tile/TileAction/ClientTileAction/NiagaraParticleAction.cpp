// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileAction/ClientTileAction/NiagaraParticleAction.h"
#include "Player/PlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Tile/Tile.h"
//#include "NiagaraComponent.h"

void UNiagaraParticleAction::Active(APlayerCharacter* PlayerCharacter) {
	Super::Active(PlayerCharacter);

    if (NiagaraFX)
    {
        ATile* Tile = Cast<ATile>(GetOuter());
        if (IsValid(Tile) == false) return;
        UWorld* World = Tile ? Tile->GetWorld() : GetWorld();
        if (!IsValid(World)) return;

        // 스폰하고 컴포넌트 받아오기
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            World,             // WorldContextObject: World 또는 World 관련 객체 사용
            NiagaraFX,         // UNiagaraSystem*
            Tile->GetActorLocation(),
            Tile->GetActorRotation(),
            FVector(1.0f),
            true,              // bAutoDestroy
            true,
            ENCPoolMethod::None,
            true               // bAutoActivate
        );
    }
}