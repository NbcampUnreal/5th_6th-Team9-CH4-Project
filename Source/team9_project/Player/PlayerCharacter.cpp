#include "Player/PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"
#include "State/PlayerStateMachine.h"
#include "State/StateBase.h"
#include "EnhancedInputComponent.h"
#include "CameraPawn.h"
#include "Tile/TileManagerActor.h"
#include "Tile/Tile.h"
#include "Net/UnrealNetwork.h"


APlayerCharacter::APlayerCharacter() :
	MoveSpeed(300.f),
	MoveDuration(0.f),
	MoveElapsed(0.f),
	remainingMove(0),
	bIsMoving(false),
	bIsUsingItem(false),
	bIsDie(false),
	bIsHit(false)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


}

void APlayerCharacter::InitCharacter(ACameraPawn* InCameraPawn, AMyPlayerState* InPlyaerState)
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::InitCharacter"));
	CameraPawn = InCameraPawn;
	SetOwner(CameraPawn);
	MyPlayerState = InPlyaerState;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(APlayerCharacter, CameraPawn);
	DOREPLIFETIME(APlayerCharacter, MyPlayerState);
	DOREPLIFETIME(APlayerCharacter, CurrentTile);
	DOREPLIFETIME(APlayerCharacter, bIsMoving);
	DOREPLIFETIME(APlayerCharacter, bIsUsingItem);
	DOREPLIFETIME(APlayerCharacter, bIsDie);
	DOREPLIFETIME(APlayerCharacter, bIsHit);
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

	return ActualDamage;
}

void APlayerCharacter::MoveToNextNode(int DiceValue)
{
	remainingMove = DiceValue;
	ATileManagerActor* TileManager = ATileManagerActor::Get(GetWorld());
	CurrentIndex = MyPlayerState->GetTileIndex();

	ATile* Tile = TileManager->GetTile(CurrentIndex);
	if (IsValid(Tile))
	{
		MoveStart = Tile->GetActorLocation();
	}

	// 현재 타일에서 이동가능한 타일 배열
	TArray<ATile*> NextTiles = TileManager->GetTile(CurrentIndex)->GetNextTiles();
	MoveTarget = NextTiles[0]->GetActorLocation();

	FVector ToTarget = (MoveTarget - MoveStart).GetSafeNormal();
	FRotator TargetRotation = ToTarget.Rotation();

	SetActorRotation(TargetRotation);

	float Distance = FVector::Dist(MoveStart, MoveTarget);
	MoveDuration = Distance / MoveSpeed;
	MoveElapsed = 0.f;

	if (bPlayerLeave == false)
	{
		TileManager->PlayerLeave(CurrentIndex, this);
	}

	bPlayerLeave = true;

	// 0.01초 간격으로 이동 업데이트
	GetWorldTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&APlayerCharacter::UpdateMove,
		0.01f,
		true
	);
}

void APlayerCharacter::SetCharacterPosition()
{
	ATileManagerActor* TileManager = ATileManagerActor::Get(GetWorld());
	CurrentIndex = MyPlayerState->GetTileIndex();
	ATile* Tile = TileManager->GetTile(CurrentIndex);

	SetActorLocation(Tile->GetActorLocation());
}

void APlayerCharacter::MultiRPCMove_Implementation(int DiceValue)
{
	MoveToNextNode(DiceValue);
}

void APlayerCharacter::UpdateMove()
{
	MoveElapsed += 0.01f;

	float Alpha = FMath::Clamp(MoveElapsed / MoveDuration, 0.f, 1.f);
	FVector NewLocation = FMath::Lerp(MoveStart, MoveTarget, Alpha);
	SetActorLocation(FVector(NewLocation.X, NewLocation.Y, NewLocation.Z + 140));

	if (Alpha >= 1.f)
	{  
		ATileManagerActor* TileManager = ATileManagerActor::Get(GetWorld());
		TArray<ATile*> NextTiles = TileManager->GetTile(CurrentIndex)->GetNextTiles();

		CurrentIndex = NextTiles[0]->GetIndex();
		MyPlayerState->SetTileIndex(CurrentIndex);
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
		
		remainingMove--;
		if (remainingMove > 0) // 지나가는중
		{
			TileManager->PlayerPassed(CurrentIndex, this);
			MoveToNextNode(remainingMove);
		}
		else // 도착
		{
			TileManager->PlayerArrive(CurrentIndex, this);
			SetCurrentTile(NextTiles[0]);
			bPlayerLeave = false;
		}
	}
}

bool APlayerCharacter::OnDie()
{
	if (MyPlayerState->GetHP() <= 0)
	{
		bIsDie = true;
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