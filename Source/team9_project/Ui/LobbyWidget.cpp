#include "Ui/LobbyWidget.h"
#include "UIManagerSubsystem.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/WidgetTree.h"

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

    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS) return;

    // 현재 플레이어 배열 가져오기
    TArray<APlayerState*> Players = GS->PlayerArray;

    // 로그를 찍어서 현재 몇 명이 잡히는지 확인 (출력 창에서 확인 가능)
    // UE_LOG(LogTemp, Warning, TEXT("Current Player Count: %d"), Players.Num());

    // UI 갱신 (기존 항목 삭제 후 재생성)
    PlayerListContainer->ClearChildren();

    const int32 MaxPlayers = 4;
    for (int32 i = 0; i < MaxPlayers; ++i)
    {
        UBorder* PlayerBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
        if (PlayerBorder)
        {
            PlayerBorder->SetPadding(FMargin(20.f, 15.f));

            UTextBlock* NameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
            if (NameText)
            {
                // 플레이어 상태가 유효하고 이름이 아직 초기값("Player")이 아닐 때만 이름 표시
                FString DisplayName = TEXT("Empty Slot");
                if (Players.IsValidIndex(i) && Players[i])
                {
                    DisplayName = Players[i]->GetPlayerName();
                }

                NameText->SetText(FText::FromString(DisplayName));

                FSlateFontInfo FontInfo = NameText->GetFont();
                FontInfo.Size = 26;
                NameText->SetFont(FontInfo);

                PlayerBorder->SetContent(NameText);
            }
            PlayerListContainer->AddChild(PlayerBorder);
        }
    }

    // --- 자동 시작 및 버튼 활성화 로직 ---
    bool bIsHost = GetOwningPlayer()->HasAuthority();
    int32 CurrentCount = Players.Num();

    if (CurrentCount >= MaxPlayers)
    {
        if (bIsHost) OnActionClicked();
    }
    else
    {
        if (Btn_Action) Btn_Action->SetIsEnabled(bIsHost);
    }

    if (RoundSettingSpinner) RoundSettingSpinner->SetIsEnabled(bIsHost);
}

void ULobbyWidget::OnActionClicked()
{
    UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (!UISubsystem) return;

    if (GetOwningPlayer()->HasAuthority())
    {
        if (Btn_Action) Btn_Action->SetIsEnabled(false);
        UISubsystem->StartHostGame(TEXT("/Game/KJH/Test/MainMap"));
    }
}