#pragma once
#include "CoreMinimal.h"
#include "EGameUIState.generated.h"

UENUM(BlueprintType)
enum class EGameUIState : uint8
{
    None        UMETA(DisplayName = "None"),
    MainMenu    UMETA(DisplayName = "Main Menu"),
    InGame      UMETA(DisplayName = "In Game"),
    Lobby       UMETA(DisplayName = "Lobby"),
    GameOver    UMETA(DisplayName = "Game Over"),
    Pause       UMETA(DisplayName = "Pause"),
    Result      UMETA(DisplayName = "Result")
};
