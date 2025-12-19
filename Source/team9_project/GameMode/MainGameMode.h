#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	virtual void Logout(AController* Exiting) override;

	//주사위 굴리기
	int32 ThrowDice(AController* ThrowingPlayer);

private:
	//게임중인 플레이어 목록
	UPROPERTY()
	TSet<AController*> PlayersInGame;
};
