#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
void AT9_MiniGamePlayerControllerBase::ShowReadyScreen()
{
}

void AT9_MiniGamePlayerControllerBase::ShowResultScreen()
{
}

void AT9_MiniGamePlayerControllerBase::ClearWidget() 
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

void AT9_MiniGamePlayerControllerBase::SetInputEnabled(bool bEnable)
{
	SetIgnoreMoveInput(!bEnable);
	SetIgnoreLookInput(!bEnable);
}