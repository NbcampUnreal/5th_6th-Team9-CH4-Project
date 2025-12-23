#include "Ui/MainTitleWidget.h"
#include "UIManagerSubsystem.h"
#include "Ui/Test/MyTestGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_ConfirmName) Btn_ConfirmName->OnClicked.AddDynamic(this, &UMainTitleWidget::OnConfirmNameClicked);
    if (Btn_CreateGame) Btn_CreateGame->OnClicked.AddDynamic(this, &UMainTitleWidget::OnCreateGameClicked);
    if (Btn_JoinGame) Btn_JoinGame->OnClicked.AddDynamic(this, &UMainTitleWidget::OnJoinGameClicked);
    if (Btn_Quit) Btn_Quit->OnClicked.AddDynamic(this, &UMainTitleWidget::OnQuitClicked);

    // 저장된 이름이 있다면 미리 표시
    if (UMyTestGameInstance* GI = Cast<UMyTestGameInstance>(GetGameInstance()))
    {
        if (!GI->SavedPlayerName.IsEmpty() && GI->SavedPlayerName != TEXT("Player"))
        {
            // 1. 입력창에 저장된 이름 표시
            if (Input_Name)
            {
                Input_Name->SetText(FText::FromString(GI->SavedPlayerName));
            }

            // 2. 현재 이름 표시 텍스트 업데이트 (있으면)
            if (Text_CurrentName)
            {
                Text_CurrentName->SetText(FText::FromString(FString::Printf(TEXT("Name Current: %s"), *GI->SavedPlayerName)));
            }
        }
    }
}

void UMainTitleWidget::OnConfirmNameClicked()
{
    FString NewName = Input_Name ? Input_Name->GetText().ToString().TrimStartAndEnd() : TEXT("");
    if (NewName.IsEmpty()) NewName = TEXT("Player");

    if (UMyTestGameInstance* GI = Cast<UMyTestGameInstance>(GetGameInstance()))
    {
        GI->SavedPlayerName = NewName;  // 실제 저장

        if (Text_CurrentName)
        {
            Text_CurrentName->SetText(FText::FromString(FString::Printf(TEXT("Name Current: %s"), *NewName)));
        }

        UE_LOG(LogTemp, Warning, TEXT("Name Confirmed: %s"), *NewName);  // 디버깅 로그 이름 확정
    }
}

void UMainTitleWidget::OnCreateGameClicked()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        FString PlayerName = Input_Name ? Input_Name->GetText().ToString().TrimStartAndEnd() : TEXT("Player");
        if (PlayerName.IsEmpty()) PlayerName = TEXT("Player");

        UISubsystem->StartHostGame(TEXT("/Game/KJH/Test/NewMap"), PlayerName);
    }
}

void UMainTitleWidget::OnJoinGameClicked()
{
    if (UUIManagerSubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
    {
        FString IP = Input_Port ? Input_Port->GetText().ToString() : TEXT("127.0.0.1");

        FString PlayerName = Input_Name ? Input_Name->GetText().ToString().TrimStartAndEnd() : TEXT("Player");
        if (PlayerName.IsEmpty()) PlayerName = TEXT("Player");

        UISubsystem->StartJoinGame(IP, PlayerName);
    }
}

void UMainTitleWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}