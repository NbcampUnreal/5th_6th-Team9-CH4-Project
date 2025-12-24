#include "Ui/LobbyWidget.h"
#include "UIManagerSubsystem.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/WidgetTree.h"
#include "Ui/Test/MyTestPlayerState.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_Action)
        Btn_Action->OnClicked.AddDynamic(this, &ULobbyWidget::OnActionClicked);

    // 1. 즉시 한 번 갱신 시도
    UpdatePlayerList();

    // 2. [해결책] 1초마다 UpdatePlayerList를 반복 호출하여 정보가 복제될 때마다 UI 업데이트
    GetWorld()->GetTimerManager().SetTimer(RefreshTimerHandle, this, &ULobbyWidget::UpdatePlayerList, 1.0f, true);
}

void ULobbyWidget::NativeDestruct()
{
    // 위젯이 닫히면 타이머를 반드시 해제 (메모리 관리)
    GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
    Super::NativeDestruct();
}

void ULobbyWidget::UpdatePlayerList()
{
    if (!PlayerListContainer) return;
    PlayerListContainer->ClearChildren();

    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS) return;

    const int32 MaxPlayers = 4; // 슬롯 수 (UI에 보여줄 최대 칸 수)
    const int32 RequiredPlayers = 4; // 자동 시작에 필요한 최소 인원

    TArray<APlayerState*> Players = GS->PlayerArray;

    // === 플레이어 리스트 생성 ===
    for (int32 i = 0; i < MaxPlayers; ++i)
    {
        UBorder* PlayerBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
        if (PlayerBorder)
        {
            PlayerBorder->SetBrushColor(FLinearColor(0.05f, 0.05f, 0.05f, 0.5f));
            PlayerBorder->SetPadding(FMargin(20.f, 10.f));

            UTextBlock* NameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
            if (NameText)
            {
                FString NameToDisplay = TEXT("Empty Slot");

                if (Players.IsValidIndex(i) && Players[i])
                {
                    if (AMyTestPlayerState* TestPS = Cast<AMyTestPlayerState>(Players[i]))
                    {
                        NameToDisplay = TestPS->DisplayName.IsEmpty() ? TestPS->GetPlayerName() : TestPS->DisplayName;
                    }
                    else
                    {
                        NameToDisplay = Players[i]->GetPlayerName();
                    }
                }

                NameText->SetText(FText::FromString(NameToDisplay));

                FSlateFontInfo FontInfo = NameText->GetFont();
                FontInfo.Size = 24;
                NameText->SetFont(FontInfo);
                NameText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

                PlayerBorder->SetContent(NameText);
            }

            UPanelSlot* PanelSlot = PlayerListContainer->AddChild(PlayerBorder);
            if (UVerticalBoxSlot* VertSlot = Cast<UVerticalBoxSlot>(PanelSlot))
            {
                FSlateChildSize FillSize;
                FillSize.SizeRule = ESlateSizeRule::Fill;
                FillSize.Value = 1.0f;
                VertSlot->SetSize(FillSize);
                VertSlot->SetPadding(FMargin(0.f, 2.f));
                VertSlot->SetHorizontalAlignment(HAlign_Fill);
                VertSlot->SetVerticalAlignment(VAlign_Fill);
            }
        }
    }

    // === 버튼 및 자동 시작 로직 ===
    APlayerController* PC = GetOwningPlayer();
    bool bIsHost = PC && PC->HasAuthority();
    int32 CurrentCount = Players.Num();

    // 1. 4명 모이면 자동 시작 (이미 시작된 경우 중복 방지)
    if (CurrentCount >= RequiredPlayers)
    {
        if (bIsHost && !bGameStarted)
        {
            UE_LOG(LogTemp, Warning, TEXT("[Lobby] Outo Start"));
            OnActionClicked();
            bGameStarted = true; // 중복 시작 방지
        }
    }

    // 2. Start 버튼 활성화: 호스트에게만 항상 보이고 클릭 가능
    if (Btn_Action)
    {
        Btn_Action->SetIsEnabled(bIsHost);                         // 클릭 가능
        Btn_Action->SetVisibility(bIsHost ? ESlateVisibility::Visible : ESlateVisibility::Hidden); // 호스트에게만 보임
    }

    if (RoundSettingSpinner)
    {
        RoundSettingSpinner->SetIsEnabled(bIsHost);
    }
}

void ULobbyWidget::OnActionClicked()
{
    UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (!UISubsystem) return;

    if (GetOwningPlayer()->HasAuthority())
    {
        if (Btn_Action) Btn_Action->SetIsEnabled(false);
        FString PlayerName = TEXT("Player");  // 기본값

        // 현재 플레이어 이름 가져오기 (예: PlayerState에서)
        if (APlayerState* PS = GetOwningPlayerState())
        {
            if (AMyTestPlayerState* TestPS = Cast<AMyTestPlayerState>(PS))
            {
                PlayerName = TestPS->DisplayName;  // 또는 GetPlayerName()
            }
        }

        // 이름 전달
        UISubsystem->StartHostGame(TEXT("/Game/KJH/Test/MainMap"), PlayerName);
    }
}