#include "Ui/ResultWidget.h"
#include "Player/MyPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "UIManagerSubsystem.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 바인딩
    if (Btn_BackToMain)
    {
        Btn_BackToMain->OnClicked.AddDynamic(this, &UResultWidget::OnBackToMainClicked);
    }
}

void UResultWidget::SetupResults()
{
    // 중복 실행 방지 + 컨테이너 체크
    if (bResultsShown || !RankListContainer)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ResultWidget] Already shown or no container"));
        return;
    }

    bResultsShown = true;
    RankListContainer->ClearChildren();

    // 1. GameState에서 모든 PlayerState 가져오기
    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS)
    {
        UE_LOG(LogTemp, Error, TEXT("[ResultWidget] No GameState found"));
        return;
    }

    TArray<AMyPlayerState*> PlayerStates;
    for (APlayerState* PS : GS->PlayerArray)
    {
        if (AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS))
        {
            if (MyPS->GetPlayerNumber() > 0)  // 유효한 플레이어만
            {
                PlayerStates.Add(MyPS);
            }
        }
    }

    if (PlayerStates.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ResultWidget] No valid players found"));
        return;
    }

    // 2. 서버 점수(CurrentScore) 기준으로 정확히 정렬
    // 기준: 점수 내림차순 → 동률 시 PlayerNumber 오름차순 (낮은 번호가 상위)
    PlayerStates.Sort([this](const AMyPlayerState& A, const AMyPlayerState& B) -> bool
        {
            if (A.CurrentScore == B.CurrentScore)
            {
                return A.PlayerNumber < B.PlayerNumber;  // 동률: 낮은 번호 우선
            }
            return A.CurrentScore > B.CurrentScore;  // 점수: 높을수록 상위
        });

    UE_LOG(LogTemp, Log, TEXT("[ResultWidget] Sorted %d players by server score"), PlayerStates.Num());

    // 3. 1위 이름 강조 (선택사항)
    if (Text_WinnerName && PlayerStates.Num() > 0)
    {
        AMyPlayerState* Winner = PlayerStates[0];
        FString WinnerName = Winner->DisplayName.IsEmpty() ? Winner->GetPlayerName() : Winner->DisplayName;
        Text_WinnerName->SetText(FText::FromString(FString::Printf(TEXT("No.1: %s (Scor: %d)"),
            *WinnerName, Winner->CurrentScore)));
        Text_WinnerName->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.8f, 0.2f)));  // 금색
    }

    // 4. 순위별 행 생성 (1위부터 순서대로)
    for (int32 Rank = 0; Rank < PlayerStates.Num(); ++Rank)
    {
        AMyPlayerState* PS = PlayerStates[Rank];

        // 플레이어 이름 가져오기
        FString PlayerName = PS->DisplayName.IsEmpty() ? PS->GetPlayerName() : PS->DisplayName;
        if (PlayerName.IsEmpty()) PlayerName = FString::Printf(TEXT("Player %d"), PS->PlayerNumber);

        // 순위 행 생성
        CreateRankRow(Rank + 1, PlayerName, PS->CurrentScore, RankListContainer);

        UE_LOG(LogTemp, Log, TEXT("[ResultWidget] Rank %d: %s (Score: %d, Number: %d)"),
            Rank + 1, *PlayerName, PS->CurrentScore, PS->PlayerNumber);
    }

    UE_LOG(LogTemp, Log, TEXT("[ResultWidget] Setup completed for %d players"), PlayerStates.Num());
}

void UResultWidget::CreateRankRow(int32 Rank, const FString& PlayerName, int32 Score, UPanelWidget* Container)
{
    if (!Container) return;

    UBorder* RowBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
    if (!RowBorder) return;

    // 배경색 설정 (1위 금색, 2위 은색 등)
    FLinearColor BgColor;
    switch (Rank)
    {
    case 1: BgColor = FLinearColor(1.0f, 0.8f, 0.2f, 0.9f); break;   // 금색
    case 2: BgColor = FLinearColor(0.7f, 0.7f, 0.7f, 0.9f); break;   // 은색
    case 3: BgColor = FLinearColor(0.8f, 0.6f, 0.4f, 0.9f); break;   // 동색
    default: BgColor = FLinearColor(0.15f, 0.15f, 0.15f, 0.8f); break;
    }
    RowBorder->SetBrushColor(BgColor);
    RowBorder->SetPadding(FMargin(20.0f, 12.0f));

    UTextBlock* ResultText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    if (!ResultText) return;

    // 텍스트 내용
    FString RankText = FString::Printf(TEXT("%d위: %s"), Rank, *PlayerName);
    if (Score > 0)
    {
        RankText += FString::Printf(TEXT(" (점수: %d)"), Score);
    }
    ResultText->SetText(FText::FromString(RankText));

    ResultText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    RowBorder->SetContent(ResultText);

    UPanelSlot* PanelSlot = Container->AddChild(RowBorder);
    if (UVerticalBoxSlot* VertSlot = Cast<UVerticalBoxSlot>(PanelSlot))
    {
        VertSlot->SetPadding(FMargin(0.0f, 8.0f));
        VertSlot->SetHorizontalAlignment(HAlign_Fill);
        VertSlot->SetVerticalAlignment(VAlign_Center);
    }
}
void UResultWidget::OnBackToMainClicked()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        UE_LOG(LogTemp, Log, TEXT("[ResultWidget] Back to MainTitle requested"));
        UISubsystem->ReturnToMainTitle();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[ResultWidget] UIManagerSubsystem not found"));
    }
}