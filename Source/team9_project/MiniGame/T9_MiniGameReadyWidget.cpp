#include "MiniGame/T9_MiniGameReadyWidget.h"
#include "MiniGame/T9_MiniGamePlayerControllerBase.h"
#include "MiniGame/T9_MiniGameStateBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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
        SetGameImageText(GS->GetGameTexture());
        SetGameDescriptionText(GS->GetGameDescription());
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

void UT9_MiniGameReadyWidget::SetGameImageText(UTexture2D* NewTexture)
{
    if (GameImage)
    {
        GameImage->SetBrushFromTexture(NewTexture);
    }
}

void UT9_MiniGameReadyWidget::SetGameDescriptionText(const FText& NewText)
{
    if (GameDescription)
    {
        GameDescription->SetText(NewText);
    }
}
