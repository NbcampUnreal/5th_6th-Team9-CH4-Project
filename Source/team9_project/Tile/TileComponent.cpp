// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileComponent.h"
#include "Tile/TileAction.h"
#include "Tile/Tile.h"

// Sets default values for this component's properties
UTileComponent::UTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UTileComponent::OnRegister()
{
	Super::OnRegister();
	ATile* Tile = Cast<ATile>(GetOwner());
	checkf(IsValid(Tile), TEXT("Tile is Not Valid, UTileComponent::OnRegister"));
	SetOnPlayerArriveTileActions();
	SetOnPlayerRollDiceTileActions();
	SetOnPlayerLeaveTileActions();
	SetOnPlayerUseItemTileActions();
	SetOnPlayerPassedTileActions();
}

void UTileComponent::OnUnregister()
{
	Super::OnUnregister();
	_TileActionInstance.Empty();
}

void UTileComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	_TileActionInstance.Empty();
}

void UTileComponent::SetOnPlayerArriveTileActions()
{
	ATile* Tile = Cast<ATile>(GetOwner());
	for (TSubclassOf<UTileAction>& SubTileAction : _OnPlayerArriveTileActions)
	{
		if (!SubTileAction) continue;
		UTileAction* Inst = NewObject<UTileAction>(Tile, SubTileAction);
		if (IsValid(Inst))
		{
			_TileActionInstance.Add(Inst);
			Tile->OnPlayerArrive.AddUObject(Inst, &UTileAction::Active);
		}
	}
}

void UTileComponent::SetOnPlayerRollDiceTileActions()
{
	ATile* Tile = Cast<ATile>(GetOwner());
	for (TSubclassOf<UTileAction>& SubTileAction : _OnPlayerRollDiceTileActions)
	{
		if (!SubTileAction) continue;
		UTileAction* Inst = NewObject<UTileAction>(Tile, SubTileAction);
		if (IsValid(Inst))
		{
			_TileActionInstance.Add(Inst);
			Tile->OnPlayerRollDice.AddUObject(Inst, &UTileAction::Active);
		}
	}
}

void UTileComponent::SetOnPlayerLeaveTileActions()
{
	ATile* Tile = Cast<ATile>(GetOwner());
	for (TSubclassOf<UTileAction>& SubTileAction : _OnPlayerLeaveTileActions)
	{
		if (!SubTileAction) continue;
		UTileAction* Inst = NewObject<UTileAction>(Tile, SubTileAction);
		if (IsValid(Inst))
		{
			_TileActionInstance.Add(Inst);
			Tile->OnPlayerLeave.AddUObject(Inst, &UTileAction::Active);
		}
	}
}

void UTileComponent::SetOnPlayerUseItemTileActions()
{
	ATile* Tile = Cast<ATile>(GetOwner());
	for (TSubclassOf<UTileAction>& SubTileAction : _OnPlayerUseItemTileActions)
	{
		if (!SubTileAction) continue;
		UTileAction* Inst = NewObject<UTileAction>(Tile, SubTileAction);
		if (IsValid(Inst))
		{
			_TileActionInstance.Add(Inst);
			Tile->OnPlayerUseItem.AddUObject(Inst, &UTileAction::Active);
		}
	}
}

void UTileComponent::SetOnPlayerPassedTileActions()
{
	ATile* Tile = Cast<ATile>(GetOwner());
	for (TSubclassOf<UTileAction>& SubTileAction : _OnPlayerPassedTileActions)
	{ 
		if (!SubTileAction) continue;
		UTileAction* Inst = NewObject<UTileAction>(Tile, SubTileAction);
		if (IsValid(Inst))
		{
			_TileActionInstance.Add(Inst);
			Tile->OnPlayerPassed.AddUObject(Inst, &UTileAction::Active);
		}
	}
}


// Called every frame
void UTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

