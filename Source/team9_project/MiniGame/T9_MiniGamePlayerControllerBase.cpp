#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "MiniGame/T9_MiniGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

void AT9_MiniGamePlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_MiniGamePlayerControllerBase::CreateUI()
{

}

void AT9_MiniGamePlayerControllerBase::ShowReadyUI()
{

}

void AT9_MiniGamePlayerControllerBase::ShowResultUI()
{

}

void AT9_MiniGamePlayerControllerBase::ShowInGameUI()
{

}

void AT9_MiniGamePlayerControllerBase::RemoveAllUI()
{

}

void AT9_MiniGamePlayerControllerBase::ChangeUI(EMiniGamePhase NewPhase)
{

}

void AT9_MiniGamePlayerControllerBase::SetInputEnabled(bool bEnable)
{
	SetIgnoreMoveInput(!bEnable);
	SetIgnoreLookInput(!bEnable);
}

void AT9_MiniGamePlayerControllerBase::ServerRPCUpdatePlayerReady_Implementation()
{
	AT9_MiniGameModeBase* GM = GetWorld()->GetAuthGameMode<AT9_MiniGameModeBase>();
	if (GM)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckGame"));
		GM->CheckStartGame();
	}
}
