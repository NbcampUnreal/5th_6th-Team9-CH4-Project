#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "MiniGame/T9_MiniGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MiniGame/T9_MiniGameCharacterBase.h"
#include "MiniGame/T9_MiniGameReadyWidget.h"
#include "MiniGame/T9_MiniGameResultWidget.h"
#include "Player/MyPlayerState.h"
#include "Net/UnrealNetwork.h"

void AT9_MiniGamePlayerControllerBase::ServerRPCUpdatePlayerReady_Implementation()
{
	AT9_MiniGameStateBase* GS =GetWorld()->GetGameState<AT9_MiniGameStateBase>();
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (GS&&PS)
	{
		int32 PlayerNumber = PS->GetPlayerNumber();

		UE_LOG(LogTemp, Warning,
			TEXT("ServerRPCUpdatePlayerReady | PlayerNumber = %d"),
			PlayerNumber
		);
		GS->SetPlayerReady(PlayerNumber, true);
	}
}


void AT9_MiniGamePlayerControllerBase::SetInputEnabled(bool bEnable)
{
	APawn* ControlledPawn = GetPawn();
	AT9_MiniGameCharacterBase* MiniGameCharacter = Cast<AT9_MiniGameCharacterBase>(ControlledPawn);
	if (MiniGameCharacter)
	{
		MiniGameCharacter->SetAcceptInput(bEnable);
	}
}

void AT9_MiniGamePlayerControllerBase::ClientRPCUpdatePlayerList_Implementation()
{
	if (ReadyWidgetInstance)
	{
		AT9_MiniGameStateBase* GS = GetWorld()->GetGameState<AT9_MiniGameStateBase>();
		if (GS) 
		{
			FString List;
			for (const FMiniGamePlayerReady& Info : GS->PlayerReadys)
			{
				List += FString::Printf(TEXT("%d : %s\n"),Info.PlayerId,Info.bReady ? TEXT("Ready") : TEXT("Not Ready"));
			}
			ReadyWidgetInstance->PlayerList->SetText(FText::FromString(List));
		}
	}
}

void AT9_MiniGamePlayerControllerBase::ClientRPCWinner_Implementation()
{
	if (ResultWidgetInstance)
	{
		AT9_MiniGameStateBase* GS = GetWorld()->GetGameState<AT9_MiniGameStateBase>();
		if (GS)
		{
			if (GS->WinnerPlayer != 0)
			{
				FString List = FString::Printf(TEXT("Win : %d"), GS->WinnerPlayer);
				ResultWidgetInstance->Result->SetText(FText::FromString(List));
			}
		}
	}
}

void AT9_MiniGamePlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		CreateUI();
		ShowReadyUI();
	}
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
