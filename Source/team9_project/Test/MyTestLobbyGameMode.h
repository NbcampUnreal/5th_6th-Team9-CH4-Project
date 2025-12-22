#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyTestLobbyGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMyTestLobbyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyTestLobbyGameMode();  // 持失切 識情

    virtual void PostLogin(APlayerController* NewPlayer) override;
};