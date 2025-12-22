#include "Ui/MainTitleWidget.h"
#include "UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_CreateGame) Btn_CreateGame->OnClicked.AddDynamic(this, &UMainTitleWidget::OnCreateGameClicked);
    if (Btn_JoinGame) Btn_JoinGame->OnClicked.AddDynamic(this, &UMainTitleWidget::OnJoinGameClicked);
    if (Btn_Quit) Btn_Quit->OnClicked.AddDynamic(this, &UMainTitleWidget::OnQuitClicked);
}

void UMainTitleWidget::OnCreateGameClicked()
{
    // Subsystem을 통해 중앙 집중식으로 게임 시작 제어
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {        
        // 실제 로비로 사용할 레벨 이름을 넣으세요 (예: UISubsystem->StartHostGame(TEXT("/Game/Maps/LobbyMap"));)
        UISubsystem->StartHostGame(TEXT("/Game/KJH/Test/NewMap"));
    }
}

void UMainTitleWidget::OnJoinGameClicked()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        FString IP = Input_Name ? Input_Name->GetText().ToString() : TEXT("127.0.0.1");
        UISubsystem->StartJoinGame(IP);
    }
}

void UMainTitleWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}