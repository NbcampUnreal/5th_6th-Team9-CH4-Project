#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class TEAM9_PROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	virtual void Logout(AController* Exiting) override;

	//이름 지정하기
	void SetPlayerName(AController* Exiting, const FString& NewPlayerName); 

private:
	//로비에 있는 플레이어 목록
	UPROPERTY()
	TSet<AController*> PlayersInLobby;
};
