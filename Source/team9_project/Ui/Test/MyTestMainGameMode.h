#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyTestMainGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMyTestMainGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    virtual void PostLogin(APlayerController* NewPlayer) override;
};