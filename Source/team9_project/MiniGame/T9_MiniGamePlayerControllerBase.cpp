#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "MiniGame/T9_MiniGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MiniGame/T9_MiniGameReadyWidget.h"
#include "MiniGame/T9_MiniGameResultWidget.h"
#include "Net/UnrealNetwork.h"

void AT9_MiniGamePlayerControllerBase::ServerRPCUpdatePlayerReady_Implementation()
{
	AT9_MiniGameModeBase* GM = GetWorld()->GetAuthGameMode<AT9_MiniGameModeBase>();
	if (GM)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckGame"));
		GM->CheckStartGame();
	}
}


void AT9_MiniGamePlayerControllerBase::SetInputEnabled(bool bEnable)
{
	SetIgnoreMoveInput(!bEnable);
	SetIgnoreLookInput(!bEnable);
}

void AT9_MiniGamePlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_MiniGamePlayerControllerBase::CreateUI()
{
	if (IsLocalController())
	{
		if (ReadyWidgetClass)
		{
			ReadyWidgetInstance = CreateWidget<UT9_MiniGameReadyWidget>(this, ReadyWidgetClass);
		}
		if (ResultWidgetClass)
		{
			ResultWidgetInstance = CreateWidget<UT9_MiniGameResultWidget>(this, ResultWidgetClass);
		}
	}
}

void AT9_MiniGamePlayerControllerBase::ShowReadyUI()
{
	RemoveAllUI();
	if (ReadyWidgetInstance)
	{
		ReadyWidgetInstance->AddToViewport();
	}
	else
	{
		CreateUI();
		ReadyWidgetInstance->AddToViewport();
	}
}

void AT9_MiniGamePlayerControllerBase::ShowResultUI()
{
	RemoveAllUI();
	if (ResultWidgetInstance)
	{
		ResultWidgetInstance->AddToViewport();
	}
	else
	{
		CreateUI();
		ResultWidgetInstance->AddToViewport();
	}
}

void AT9_MiniGamePlayerControllerBase::ShowInGameUI()
{
}

void AT9_MiniGamePlayerControllerBase::RemoveAllUI()
{
	if (ReadyWidgetInstance)
	{
		ReadyWidgetInstance->RemoveFromParent();
	}

	if (ResultWidgetInstance)
	{
		ResultWidgetInstance->RemoveFromParent();
	}
}

void AT9_MiniGamePlayerControllerBase::ChangeUI(EMiniGamePhase NewPhase)
{
}
