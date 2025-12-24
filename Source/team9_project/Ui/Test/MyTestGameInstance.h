#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Ui/EGameUIState.h"
#include "MyTestGameInstance.generated.h"

UCLASS()
class TEAM9_PROJECT_API UMyTestGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMyTestGameInstance();

    // 게임 시작 시 자동 실행 → UI 서브시스템에 위젯 등록
    virtual void Init() override;

    // 현재 라운드 설정/조회 (GameHUD 등에서 사용)
    void SetCurrentRound(int32 CurrentR) { CurrentRound = CurrentR; }
    int32 GetCurrentRound() const { return CurrentRound; }
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FString SavedPlayerName = TEXT("Player"); // 이름을 저장할 변수 추가

protected:
    // 에디터에서 상태별로 사용할 위젯을 연결하는 맵
    // 예) MainMenu → WBP_MainTitle, InGame → WBP_GameHUD
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Settings")
    TMap<EGameUIState, TSubclassOf<UUserWidget>> TestUIWidgetMap;

private:
    // 현재 진행 중인 라운드
    UPROPERTY()
    int32 CurrentRound = 1;
};