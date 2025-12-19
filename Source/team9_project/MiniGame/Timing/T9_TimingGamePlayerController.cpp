#include "MiniGame/Timing/T9_TimingGamePlayerController.h"
#include "MiniGame/T9_MiniGameReadyWidget.h"
#include "MiniGame/T9_MiniGameResultWidget.h"
#include "MiniGame/Timing/T9_TimingGameState.h"
#include "MiniGame/Timing/T9_TimingGameMode.h"
#include "MiniGame/Timing/T9_TimingInGameWidget.h"
#include "Net/UnrealNetwork.h"

void AT9_TimingGamePlayerController::ServerRPC_StopTimer_Implementation()
{
	AT9_TimingGameState* GS = GetWorld()->GetGameState<AT9_TimingGameState>();
	if (!GS) return;

	GS->AddPlayerPress(1, GetWorld()->GetTimeSeconds());//PlayerId Change
}

void AT9_TimingGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_TimingGamePlayerController::CreateUI()
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
		if (InGameWidgetClass)
		{
			InGameWidgetInstance = CreateWidget<UT9_TimingInGameWidget>(this, InGameWidgetClass);
		}
	}
}

void AT9_TimingGamePlayerController::ShowInGameUI()
{
	RemoveAllUI();
	if (InGameWidgetInstance)
	{
		InGameWidgetInstance->AddToViewport();
	}
	else
	{
		CreateUI();
		InGameWidgetInstance->AddToViewport();
	}
}

void AT9_TimingGamePlayerController::RemoveAllUI()
{
	if (ReadyWidgetInstance)
	{
		ReadyWidgetInstance->RemoveFromParent();
	}

	if (InGameWidgetInstance)
	{
		InGameWidgetInstance->RemoveFromParent();
	}

	if (ResultWidgetInstance)
	{
		ResultWidgetInstance->RemoveFromParent();
	}
}

void AT9_TimingGamePlayerController::ChangeUI(EMiniGamePhase NewPhase)
{
	UE_LOG(LogTemp, Error, TEXT("ChangeUI"));
	if (!IsLocalController())
	{
		return;
	}
	switch (NewPhase)
	{
	case EMiniGamePhase::Ready:
		ShowReadyUI();
		SetInputEnabled(false);
		break;

	case EMiniGamePhase::Playing:
		ShowInGameUI();
		SetInputEnabled(false);
		break;

	case EMiniGamePhase::Result:
		ShowResultUI();
		SetInputEnabled(false);
		break;
	}
}
