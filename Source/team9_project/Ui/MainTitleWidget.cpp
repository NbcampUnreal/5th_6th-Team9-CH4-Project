// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/MainTitleWidget.h"
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
    // [TODO] GameInstance에 이름 저장 로직 추가 (Input_Name->GetText())

    // 리슨 서버로 맵 열기
    UGameplayStatics::OpenLevel(this, FName("MainMap"), true, TEXT("listen"));
}

void UMainTitleWidget::OnJoinGameClicked()
{
    // 실제 구현시 IP 입력 팝업 필요. 여기서는 로컬호스트로 가정.
    UGameplayStatics::OpenLevel(this, FName("127.0.0.1"));
}

void UMainTitleWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}