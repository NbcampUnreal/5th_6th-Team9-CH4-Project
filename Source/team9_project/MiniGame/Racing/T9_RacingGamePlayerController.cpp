#include "MiniGame/Racing/T9_RacingGamePlayerController.h"
#include "MiniGame/T9_MiniGameReadyWidget.h"
#include "MiniGame/T9_MiniGameResultWidget.h"
#include "MiniGame/Racing/T9_RacingInGameWidget.h"
#include "MiniGame/Racing/T9_RacingGameMode.h"
#include "MiniGame/Racing/T9_RacingGameState.h"

void AT9_RacingGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AT9_RacingGamePlayerController::CreateUI()
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
			InGameWidgetInstance = CreateWidget<UT9_RacingInGameWidget>(this, InGameWidgetClass);
		}
	}
}

void AT9_RacingGamePlayerController::ShowInGameUI()
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

void AT9_RacingGamePlayerController::RemoveAllUI()
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

void AT9_RacingGamePlayerController::ChangeUI(EMiniGamePhase NewPhase)
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
		SetInputEnabled(true);
		break;

	case EMiniGamePhase::Result:
		ShowResultUI();
		SetInputEnabled(false);
		break;
	}
}
