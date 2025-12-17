#include "MiniGame/T9_MiniGameReadyScreenWidget.h"
#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "Components/Button.h"

void UT9_MiniGameReadyScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
    if (ReadyButton)
    {
        ReadyButton->OnClicked.RemoveAll(this);
        ReadyButton->OnClicked.AddDynamic(this, &UT9_MiniGameReadyScreenWidget::OnReadyButtonClicked);
        ReadyButton->SetIsEnabled(true);
    }
}

void UT9_MiniGameReadyScreenWidget::OnReadyButtonClicked()
{
    AT9_MiniGamePlayerControllerBase* PC = Cast<AT9_MiniGamePlayerControllerBase>(GetOwningPlayer());
    if (PC)
    {
        UE_LOG(LogTemp, Error, TEXT("ButtonClick"));
        PC->ServerRPCUpdatePlayerReady();
    }
}
