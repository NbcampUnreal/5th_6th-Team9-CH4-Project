// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/TileComponent.h"
#include "Tile/TileAction.h"
#include "Tile./Tile.h"

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

void UTileComponent::BeginDestroy()
{
	_TileInstance.Empty();
	Super::BeginDestroy();
}

void UTileComponent::OnRegister()
{
	Super::OnRegister();

	ATile* Tile = Cast<ATile>(GetOwner());
	checkf(IsValid(Tile), TEXT("Tile is Not Valid, UTileComponent::OnRegister"));

	for (TSubclassOf<UTileAction>& SubTileAction : _TileActions)
	{
		if (!SubTileAction) continue;
		UTileAction* Inst = NewObject<UTileAction>(this, SubTileAction);
		if (IsValid(Inst))
		{
			_TileInstance.Add(Inst);
		}
	}
}


// Called every frame
void UTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

