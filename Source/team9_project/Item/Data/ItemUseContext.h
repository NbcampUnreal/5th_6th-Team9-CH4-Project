#pragma once

#include "CoreMinimal.h"
#include "ItemUseContext.generated.h"

//아이템 사용시 전달되는 정보
USTRUCT(Blueprintable)
struct FItemUseContext
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Aimdirection = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetTileIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FinalLocation = FVector::ZeroVector;
	
};
