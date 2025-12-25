#include "Ui/ResultWidget.h"
#include "Player/MyPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBoxSlot.h"
#include "UIManagerSubsystem.h"
#include "Blueprint/WidgetTree.h"

void UResultWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (Btn_BackToMain)
    {
        Btn_BackToMain->OnClicked.AddDynamic(this, &UResultWidget::OnBackToMainClicked);
    }
}

void UResultWidget::SetupResults()
{
    if (bResultsShown || !RankListContainer) return;
    bResultsShown = true;
    RankListContainer->ClearChildren();

    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS) return;

    TArray<AMyPlayerState*> PlayerStates;
    for (APlayerState* PS : GS->PlayerArray)
    {
        if (AMyPlayerState* TestPS = Cast<AMyPlayerState>(PS))
        {
            PlayerStates.Add(TestPS);
        }
    }

    PlayerStates.Sort([](const AMyPlayerState& A, const AMyPlayerState& B)
        {
            return A.FinalRank < B.FinalRank;
        });

    for (AMyPlayerState* TargetPS : PlayerStates)
    {
        UBorder* RowBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
        UTextBlock* ResultText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

        if (RowBorder && ResultText)
        {
            FLinearColor BgColor = (TargetPS->FinalRank == 1) ? FLinearColor(1.0f, 0.8f, 0.2f, 0.8f) : FLinearColor(0.1f, 0.1f, 0.1f, 0.6f);
            RowBorder->SetBrushColor(BgColor);
            RowBorder->SetPadding(FMargin(20.f, 10.f));

            FString PName = TargetPS->DisplayName.IsEmpty() ? TargetPS->GetPlayerName() : TargetPS->DisplayName;
            FString Info = FString::Printf(TEXT("%dÀ§: %s (Á¡¼ö: %d)"), TargetPS->FinalRank, *PName, TargetPS->CurrentScore);
            ResultText->SetText(FText::FromString(Info));

            FSlateFontInfo FontInfo = ResultText->GetFont();
            FontInfo.Size = 22;
            ResultText->SetFont(FontInfo);
            ResultText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

            RowBorder->SetContent(ResultText);

            UPanelSlot* PanelSlot = RankListContainer->AddChild(RowBorder);
            if (UVerticalBoxSlot* VertSlot = Cast<UVerticalBoxSlot>(PanelSlot))
            {
                VertSlot->SetPadding(FMargin(0.f, 5.f));
                VertSlot->SetHorizontalAlignment(HAlign_Fill);
            }
        }
    }
}

void UResultWidget::OnBackToMainClicked()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UISubsystem->ReturnToMainTitle();
    }
}