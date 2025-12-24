#include "MiniGame/T9_MiniGameStateBase.h"
#include "MiniGame/T9_MiniGameModeBase.h"
#include "Player/MyPlayerState.h"
#include "Net/UnrealNetwork.h"

void AT9_MiniGameStateBase::SetPhase(EMiniGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("SetPhase"));
	CurrentPhase = NewPhase;
}

void AT9_MiniGameStateBase::OnRep_Phase()
{
	OnPhaseChanged(CurrentPhase);
	UE_LOG(LogTemp, Error, TEXT("OnRep_Phase"));
}

void AT9_MiniGameStateBase::OnRep_PlayerReady()
{
	//UI업데이트
	UE_LOG(LogTemp, Error, TEXT("OnRep_PlayerReady"));
}

void AT9_MiniGameStateBase::InitializePlayerReadys()
{
	if (HasAuthority())
	{
		PlayerReadys.Empty();

		for (APlayerState* PS : PlayerArray)
		{
			AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS);

			if (!MyPS)
			{
				continue;
			}

			FMiniGamePlayerReady ReadyInfo;
			ReadyInfo.PlayerId = MyPS->GetPlayerNumber();
			ReadyInfo.bReady = false;

			PlayerReadys.Add(ReadyInfo);
		}
	}
}

void AT9_MiniGameStateBase::SetPlayerReady(int32 PlayerNumber, bool bReady)
{
	if (HasAuthority())
	{
		for (FMiniGamePlayerReady& Info : PlayerReadys)
		{
			if (Info.PlayerId == PlayerNumber)
			{
				Info.bReady = bReady;

				UE_LOG(LogTemp, Warning,
					TEXT("[GameState] PlayerNumber %d Ready = %d"),
					PlayerNumber,
					bReady
				);
				break;
			}
		}
		AT9_MiniGameModeBase* GM = GetWorld()->GetAuthGameMode<AT9_MiniGameModeBase>();
		if (GM)
		{

			GM->CheckStartGame();
		}
	}
}

void AT9_MiniGameStateBase::AddPlayerResult(int32 PlayerId, int32 Score)
{
}

void AT9_MiniGameStateBase::OnPhaseChanged(EMiniGamePhase NewPhase)
{

}

void AT9_MiniGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT9_MiniGameStateBase, PlayerResults);
	DOREPLIFETIME(AT9_MiniGameStateBase, CurrentPhase);
	DOREPLIFETIME(AT9_MiniGameStateBase, PlayerReadys);
}