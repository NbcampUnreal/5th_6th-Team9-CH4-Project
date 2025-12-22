#pragma once
#include "CoreMinimal.h"
#include "EGameUIState.generated.h"

UENUM(BlueprintType)
enum class EGameUIState : uint8
{
    None UMETA(DisplayName = "None"),
    MainMenu UMETA(DisplayName = "Main Menu"),
    InGame UMETA(DisplayName = "In Game"),
    Pause UMETA(DisplayName = "Pause"),
    GameOver UMETA(DisplayName = "Game Over"),
    Lobby UMETA(DisplayName = "Lobby")
};
