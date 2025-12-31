// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileAction/ServerTileAction/GainItemTileAction.h"
#include "Player/PlayerCharacter.h"
#include "Player/CameraPawn.h"
#include "Inventory/InventoryComponent.h"
#include "Item/ItemSubsystem.h"

void UGainItemTileAction::Active(APlayerCharacter* PlayerCharacter)
{
	Super::Active(PlayerCharacter);

	if (!PlayerCharacter)
	{
		return;
	}
	
	ACameraPawn* CameraPawn = Cast<ACameraPawn>(PlayerCharacter->GetOwner());
	if (!CameraPawn)
	{
		return;
	}

	UInventoryComponent* Inventory = CameraPawn->GetInventoryComponent();
	if (!Inventory)
	{
		return;
	}

	// ItemSubsystem 가져오기
	UWorld* World = PlayerCharacter->GetWorld();
	if (!World)
	{
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	UItemSubsystem* ItemSubsystem = GameInstance->GetSubsystem<UItemSubsystem>();
	if (!ItemSubsystem)
	{
		return;
	}

	// 지정된 개수만큼 아이템 추가
	int32 AddedCount = 0;
	for (int32 i = 0; i < ItemCount; ++i)
	{
		// 인벤토리가 꽉 찼는지 확인
		if (Inventory->IsInventoryFull())
		{
			break;
		}

		// 아이템 ID 결정 (특정 아이템 지정 또는 랜덤)
		FName ItemID = NAME_None;
		if (SpecificItemID != NAME_None && ItemSubsystem->HasItemData(SpecificItemID))
		{
			ItemID = SpecificItemID;
		}
		else
		{
			ItemID = ItemSubsystem->GetRandomItemID();
		}

		if (ItemID == NAME_None)
		{
			continue;
		}

		// 아이템 추가 (InventoryComponent의 Slots는 Replicated이므로 클라이언트에 자동 동기화)
		bool bSuccess = Inventory->AddItem(ItemID);
		if (bSuccess)
		{
			AddedCount++;
		}
		else
		{
		}
	}
	
}
