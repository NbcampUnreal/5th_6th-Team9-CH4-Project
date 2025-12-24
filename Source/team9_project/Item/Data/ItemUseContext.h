#pragma once

#include "CoreMinimal.h"
#include "ItemUseContext.generated.h"

class UInventoryComponent;

//아이템 사용시 전달되는 정보
USTRUCT(Blueprintable)
struct FItemUseContext
{
	GENERATED_BODY()

	// 아이템 소유자
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OwnerActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* OwnerInventory = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Aimdirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetTileIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FinalLocation = FVector::ZeroVector;

};
