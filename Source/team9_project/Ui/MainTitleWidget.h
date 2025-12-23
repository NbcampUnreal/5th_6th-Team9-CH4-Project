#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h" // 추가
#include "MainTitleWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UMainTitleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConfirmNameClicked(); // 이름 확정 버튼 핸들러

	UFUNCTION()
	void OnCreateGameClicked();

	UFUNCTION()
	void OnJoinGameClicked();

	UFUNCTION()
	void OnQuitClicked();

public:
	UPROPERTY(meta = (BindWidget))
	UEditableText* Input_Name;

	UPROPERTY(meta = (BindWidget))
	UEditableText* Input_Port; // IP 주소 입력창

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_ConfirmName; // 새 버튼: 이름 확정

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_CreateGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_JoinGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Quit;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_CurrentName; // 현재 확정된 이름을 보여줄 텍스트 (선택사항)
};