#include "MiniGame/T9_MiniGameReadyWidget.h"
#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UT9_MiniGameReadyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (ReadyButton)
    {
        ReadyButton->OnClicked.RemoveAll(this);
        ReadyButton->OnClicked.AddDynamic(this, &UT9_MiniGameReadyWidget::OnReadyButtonClicked);
        ReadyButton->SetIsEnabled(true);
    }

    if (AT9_MiniGameStateBase* GS = GetWorld()->GetGameState<AT9_MiniGameStateBase>())
    {
        SetGameNameText(GS->GetGameName());
    }
}

void UT9_MiniGameReadyWidget::OnReadyButtonClicked()
{
    AT9_MiniGamePlayerControllerBase* PC = Cast<AT9_MiniGamePlayerControllerBase>(GetOwningPlayer());
    if (PC)
    {
        UE_LOG(LogTemp, Error, TEXT("ButtonClick"));
        PC->ServerRPCUpdatePlayerReady();
    }
}

void UT9_MiniGameReadyWidget::SetGameNameText(const FText& NewText)
{
    if (GameName)
    {
        GameName->SetText(NewText);
    }
}
