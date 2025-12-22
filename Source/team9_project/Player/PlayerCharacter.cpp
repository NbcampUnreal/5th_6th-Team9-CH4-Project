#include "Player/PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"
#include "State/PlayerStateMachine.h"
#include "State/StateBase.h"
#include "Tile/Tile.h"
#include "EnhancedInputComponent.h"
#include "CameraPawn.h"
#include "Tile/TileManagerActor.h"


APlayerCharacter::APlayerCharacter() :
	MoveSpeed(300.f),
	MoveDuration(0.f),
	MoveElapsed(0.f),
	remainingMove(0),
	bIsMoving(false)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerCharacter::InitCharacter(ACameraPawn* InCameraPawn)
{
	CameraPawn = InCameraPawn;

	/*FVector StartPosition = InCameraPawn->GetCurrentTile()->GetActorLocation();
	SetActorLocation(StartPosition);*/

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
	{
		return 0.f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), ActualDamage);

	GetPlayerState()->SetHP(FMath::Clamp(GetPlayerState()->GetHP() - (int)ActualDamage, 0, GetPlayerState()->GetMaxHP()));
	UE_LOG(LogTemp, Warning, TEXT("Current HP : %d"), GetPlayerState()->GetHP());

	return ActualDamage;
}

void APlayerCharacter::MoveToNextNode(int DiceValue)
{
	if (!HasAuthority())
	{
		return;
	}

	remainingMove = DiceValue;

	ATileManagerActor* TileManager = ATileManagerActor::Get(GetWorld());
	CurrentIndex = MyPlayerState->GetTileIndex();
	MoveStart = TileManager->GetTile(CurrentIndex)->GetActorLocation();

	// 현재 타일에서 이동가능한 타일 배열
	TArray<ATile*> NextTiles = TileManager->GetTile(CurrentIndex)->GetNextTiles();
	MoveTarget = NextTiles[0]->GetActorLocation();

	FVector ToTarget = (MoveTarget - MoveStart).GetSafeNormal();
	FRotator TargetRotation = ToTarget.Rotation();

	SetActorRotation(TargetRotation);

	float Distance = FVector::Dist(MoveStart, MoveTarget);
	MoveDuration = Distance / MoveSpeed;
	MoveElapsed = 0.f;

	if (bIsMoving == false)
	{
		//타일 델리게이트 호출
		NextTiles[0]->PlayerLeave(this);
	}

	bIsMoving = true;

	// 0.01초 간격으로 이동 업데이트
	GetWorldTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&APlayerCharacter::UpdateMove,
		0.01f,
		true
	);
}

void APlayerCharacter::UpdateMove()
{
	MoveElapsed += 0.01f;

	float Alpha = FMath::Clamp(MoveElapsed / MoveDuration, 0.f, 1.f);
	FVector NewLocation = FMath::Lerp(MoveStart, MoveTarget, Alpha);
	SetActorLocation(FVector(NewLocation.X, NewLocation.Y, NewLocation.Z + 46));

	if (Alpha >= 1.f)
	{
		ATileManagerActor* TileManager = ATileManagerActor::Get(GetWorld());
		TArray<ATile*> NextTiles = TileManager->GetTile(CurrentIndex)->GetNextTiles();

		CurrentIndex = NextTiles[0]->GetIndex();

		GetWorldTimerManager().ClearTimer(MoveTimerHandle);

		remainingMove--;
		if (remainingMove > 0) // 지나가는중
		{
			// playerState Index 저장
			MyPlayerState->SetTileIndex(CurrentIndex);
	
			//타일 델리게이트 호출
			NextTiles[0]->PlayerPassed(this);
			UE_LOG(LogTemp, Warning, TEXT("PlayerPassed"));
			MoveToNextNode(remainingMove);
		}
		else // 도착
		{
			bIsMoving = false;
			//타일 델리게이트 호출
			NextTiles[0]->PlayerArrive(this);
			UE_LOG(LogTemp, Warning, TEXT("PlayerArrive"));
		}
	}
}

bool APlayerCharacter::OnDie()
{
	if (MyPlayerState->GetHP() <= 0)
	{
		return true;
	}
	return false;
}

void APlayerCharacter::SetPlayerState(AMyPlayerState* InPlayerState)
{
	MyPlayerState = InPlayerState;
}

AMyPlayerState* APlayerCharacter::GetPlayerState()
{
	return MyPlayerState;
}

void APlayerCharacter::SetCurrentTile(ATile* TileNode)
{
	CurrentTile = TileNode;
}

ATile* APlayerCharacter::GetCurrentTile()
{
	return CurrentTile;
}