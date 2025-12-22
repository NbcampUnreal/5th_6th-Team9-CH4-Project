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
    virtual void Init() override;

    void SetCurrentRound(int32 CurrentR) { CurrentRound = CurrentR; }
    int32 GetCurrentRound() const { return CurrentRound; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Settings")
    TMap<EGameUIState, TSubclassOf<UUserWidget>> TestUIWidgetMap;

private:
    int32 CurrentRound = 1;
};