#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyTestGameMode.generated.h"

class AMyPlayerController;

UCLASS()
class TEAM9_PROJECT_API AMyTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // 플레이어가 게임 레벨에 접속했을 때 호출
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // 주사위 로직 (서버 실행)
    void ProcessDiceThrow(AMyPlayerController* Requester);

    UFUNCTION(BlueprintCallable, Category = "GameFlow")
    void EndMatch();

protected:
    //순위 계산 (Test 임시)
    void CalculateFinalResults();
};